//=====================================================================================================
// KalmanAHRS.c
//=====================================================================================================
//
// Implementation of Kalman filter 
// Based on: https://code.google.com/p/9dof-orientation-estimation/
//
//=====================================================================================================

#include <math.h>
#include "invSqrt.h"
#include "MadgwickFullAHRS.h"


// filter parameters and states

// Kalman filter parameters
float beta = 0.151149947f; // Sqrt(3.0 / 4.0) * (PI * (10 / 180.0));
float zeta = 0.001511499f; // Sqrt(3.0 / 4.0) * (PI * (0.1 / 180.0));

// gyroscope biases error (part of filter state)
float w_bx = 0, w_by = 0, w_bz = 0;

// state & output
// reference direction of earth's magnetic field aka heading and inclination
float b_x = 1, b_z = 0;

// corrected gyro result
float w_x, w_y, w_z;
// computed flux in the earth frame
float h_x, h_y, h_z;                                    


/**
 * @brief	using Madgwick-full-filter update current orientation quaternion
 * @param	g[3] gyro measurement
 * @param	a[3] accel measurement
 * @param	m[3[ magneto measurement
 * @param	q[4] in/out - position quaternion 
 * @retval None; quaternion[4] and lobals for additional results
 */
void MadgwickFullAHRSUpdate(float g[3], float a[3], float m[3], 
														float samplePeriod, 
														/*in/out*/ float quaternion[4])
{
	float a_x=a[0];
	float a_y=a[1];
	float a_z=a[2];
	float m_x=m[0];
	float m_y=m[1];
	float m_z=m[2];
	
	// earth relative to sensor quaternion elements with initial conditions (part of filter state)
	#define SEq_1 quaternion[0]
	#define SEq_2 quaternion[1]
	#define SEq_3 quaternion[2]
	#define SEq_4 quaternion[3]

	// local system variables
	float norm;                                                            // vector norm
	float SEqDot_omega_1, SEqDot_omega_2, SEqDot_omega_3, SEqDot_omega_4;  // quaternion rate from gyroscopes elements
	float f_1, f_2, f_3, f_4, f_5, f_6;                                    // objective function elements
	float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33,              // objective function Jacobian elements
	J_41, J_42, J_43, J_44, J_51, J_52, J_53, J_54, J_61, J_62, J_63, J_64; //
	float SEqHatDot_1, SEqHatDot_2, SEqHatDot_3, SEqHatDot_4;              // estimated direction of the gyroscope error (quaternion derrivative)
	float w_err_x, w_err_y, w_err_z;                                       // estimated direction of the gyroscope error (angular)

	// axulirary variables to avoid reapeated calcualtions
	float halfSEq_1 = 0.5f * SEq_1;
	float halfSEq_2 = 0.5f * SEq_2;
	float halfSEq_3 = 0.5f * SEq_3;
	float halfSEq_4 = 0.5f * SEq_4;
	float twoSEq_1 = 2.0f * SEq_1;
	float twoSEq_2 = 2.0f * SEq_2;
	float twoSEq_3 = 2.0f * SEq_3;
	float twoSEq_4 = 2.0f * SEq_4;
	float twob_xSEq_1 = 2.0f * b_x * SEq_1;
	float twob_xSEq_2 = 2.0f * b_x * SEq_2;
	float twob_xSEq_3 = 2.0f * b_x * SEq_3;
	float twob_xSEq_4 = 2.0f * b_x * SEq_4;
	float twob_zSEq_1 = 2.0f * b_z * SEq_1;
	float twob_zSEq_2 = 2.0f * b_z * SEq_2;
	float twob_zSEq_3 = 2.0f * b_z * SEq_3;
	float twob_zSEq_4 = 2.0f * b_z * SEq_4;

	// normalise the accelerometer measurement
	norm = invSqrt(a_x * a_x + a_y * a_y + a_z * a_z);
	a_x *= norm;
	a_y *= norm;
	a_z *= norm;

	// normalise the magnetometer measurement
	norm = invSqrt(m_x * m_x + m_y * m_y + m_z * m_z);
	m_x *= norm;
	m_y *= norm;
	m_z *= norm;

	// compute the objective function and Jacobian
	f_1 = 2.0f * (SEq_2 * SEq_4 - SEq_1 * SEq_3) - a_x;
	f_2 = 2.0f * (SEq_1 * SEq_2 + SEq_3 * SEq_4) - a_y;
	f_3 = 2.0f * (0.5f - SEq_2 * SEq_2 - SEq_3 * SEq_3) - a_z;
	f_4 = 2.0f * b_x * (0.5f - SEq_3 * SEq_3 - SEq_4 * SEq_4) + 2.0f * b_z * (SEq_2 * SEq_4 - SEq_1 * SEq_3) - m_x;
	f_5 = 2.0f * b_x * (SEq_2 * SEq_3 - SEq_1 * SEq_4) + 2.0f * b_z * (SEq_1 * SEq_2 + SEq_3 * SEq_4) - m_y;
	f_6 = 2.0f * b_x * (SEq_1 * SEq_3 + SEq_2 * SEq_4) + 2.0f * b_z * (0.5f - SEq_2 * SEq_2 - SEq_3 * SEq_3) - m_z;
	J_11or24 = twoSEq_3;                                                    // J_11 negated in matrix multiplication
	J_12or23 = 2.0f * SEq_4;
	J_13or22 = twoSEq_1;                                                    // J_12 negated in matrix multiplication
	J_14or21 = twoSEq_2;
	J_32 = 2.0f * J_14or21;                                                    // negated in matrix multiplication
	J_33 = 2.0f * J_11or24;                                                    // negated in matrix multiplication
	J_41 = twob_zSEq_3;                                                     // negated in matrix multiplication
	J_42 = twob_zSEq_4;
	J_43 = 2.0f * twob_xSEq_3 + twob_zSEq_1;                                   // negated in matrix multiplication
	J_44 = 2.0f * twob_xSEq_4 - twob_zSEq_2;                                   // negated in matrix multiplication
	J_51 = twob_xSEq_4 - twob_zSEq_2;                                       // negated in matrix multiplication
	J_52 = twob_xSEq_3 + twob_zSEq_1;
	J_53 = twob_xSEq_2 + twob_zSEq_4;
	J_54 = twob_xSEq_1 - twob_zSEq_3;                                       // negated in matrix multiplication
	J_61 = twob_xSEq_3;
	J_62 = twob_xSEq_4 - 2.0f * twob_zSEq_2;
	J_63 = twob_xSEq_1 - 2.0f * twob_zSEq_3;
	J_64 = twob_xSEq_2;

	// compute the gradient (matrix multiplication)
	SEqHatDot_1 = J_14or21 * f_2 - J_11or24 * f_1 - J_41 * f_4 - J_51 * f_5 + J_61 * f_6;
	SEqHatDot_2 = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3 + J_42 * f_4 + J_52 * f_5 + J_62 * f_6;
	SEqHatDot_3 = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1 - J_43 * f_4 + J_53 * f_5 + J_63 * f_6;
	SEqHatDot_4 = J_14or21 * f_1 + J_11or24 * f_2 - J_44 * f_4 - J_54 * f_5 + J_64 * f_6;

	// normalise the gradient to estimate direction of the gyroscope error
	norm = invSqrt(SEqHatDot_1 * SEqHatDot_1 + SEqHatDot_2 * SEqHatDot_2 + SEqHatDot_3 * SEqHatDot_3 + SEqHatDot_4 * SEqHatDot_4);
	SEqHatDot_1 = SEqHatDot_1 * norm;
	SEqHatDot_2 = SEqHatDot_2 * norm;
	SEqHatDot_3 = SEqHatDot_3 * norm;
	SEqHatDot_4 = SEqHatDot_4 * norm;

	// compute angular estimated direction of the gyroscope error
	w_err_x = twoSEq_1 * SEqHatDot_2 - twoSEq_2 * SEqHatDot_1 - twoSEq_3 * SEqHatDot_4 + twoSEq_4 * SEqHatDot_3;
	w_err_y = twoSEq_1 * SEqHatDot_3 + twoSEq_2 * SEqHatDot_4 - twoSEq_3 * SEqHatDot_1 - twoSEq_4 * SEqHatDot_2;
	w_err_z = twoSEq_1 * SEqHatDot_4 - twoSEq_2 * SEqHatDot_3 + twoSEq_3 * SEqHatDot_2 - twoSEq_4 * SEqHatDot_1;

	// compute gyroscope baises
	w_bx += w_err_x * samplePeriod * zeta;
	w_by += w_err_y * samplePeriod * zeta;
	w_bz += w_err_z * samplePeriod * zeta;

	// correct gyroscope result
	w_x = g[0] - w_bx;
	w_y = g[1] - w_by;
	w_z = g[2] - w_bz;

	// compute the quaternion rate measured by gyroscopes
	SEqDot_omega_1 = -halfSEq_2 * w_x - halfSEq_3 * w_y - halfSEq_4 * w_z;
	SEqDot_omega_2 = halfSEq_1 * w_x + halfSEq_3 * w_z - halfSEq_4 * w_y;
	SEqDot_omega_3 = halfSEq_1 * w_y - halfSEq_2 * w_z + halfSEq_4 * w_x;
	SEqDot_omega_4 = halfSEq_1 * w_z + halfSEq_2 * w_y - halfSEq_3 * w_x;

	// compute then integrate the estimated quaternion rate
	SEq_1 += (SEqDot_omega_1 - (beta * SEqHatDot_1)) * samplePeriod;
	SEq_2 += (SEqDot_omega_2 - (beta * SEqHatDot_2)) * samplePeriod;
	SEq_3 += (SEqDot_omega_3 - (beta * SEqHatDot_3)) * samplePeriod;
	SEq_4 += (SEqDot_omega_4 - (beta * SEqHatDot_4)) * samplePeriod;

	// normalise quaternion
	norm = invSqrt(SEq_1 * SEq_1 + SEq_2 * SEq_2 + SEq_3 * SEq_3 + SEq_4 * SEq_4);
	SEq_1 *= norm;
	SEq_2 *= norm;
	SEq_3 *= norm;
	SEq_4 *= norm;

	// compute flux in the earth frame
	h_x = 2.0f * m_x * (0.5f - (-SEq_3) * (-SEq_3) - (-SEq_4) * (-SEq_4)) + 2.0f * m_y * (SEq_1 * (-SEq_4) + (-SEq_2) * (-SEq_3)) + 2.0f * m_z * ((-SEq_2) * (-SEq_4) - SEq_1 * (-SEq_3));
	h_y = 2.0f * m_x * ((-SEq_2) * (-SEq_3) - SEq_1 * (-SEq_4)) + 2.0f * m_y * (0.5f - (-SEq_2) * (-SEq_2) - (-SEq_4) * (-SEq_4)) + 2.0f * m_z * (SEq_1 * (-SEq_2) + (-SEq_3) * (-SEq_4));
	h_z = 2.0f * m_x * (SEq_1 * (-SEq_3) + (-SEq_2) * (-SEq_4)) + 2.0f * m_y * ((-SEq_3) * (-SEq_4) - SEq_1 * (-SEq_2)) + 2.0f * m_z * (0.5f - (-SEq_2) * (-SEq_2) - (-SEq_3) * (-SEq_3));

	// normalise the flux vector to have only components in the x and z
	b_x = sqrtf((h_x * h_x) + (h_y * h_y));
	b_z = h_z;
}

//====================================================================================================
// END OF CODE
//====================================================================================================
