/*
 * maincpp.h
 *
 *  Created on: 04.07.2018
 *      Author: mromanow
 */

#ifndef MAINPP_H_
#define MAINPP_H_

#ifdef __cplusplus
 extern "C" {
#endif

void setup(void);
void loop(void);
void TIM3_PeriodElapsedCallback(void);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
