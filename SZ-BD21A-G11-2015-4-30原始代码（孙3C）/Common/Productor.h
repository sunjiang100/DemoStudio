#ifndef		_PRODUCTOR_H_
#define		_PRODUCTOR_H_

#define			GENERAL_PRT							0
#define			GUOYEE_PRT							1
#define			SHANGHAI_SIZENG_PRT				2
#define			XIAMEN_HUIHENG_PRT				3

#define			PRODUCT_TYPE						SHANGHAI_SIZENG_PRT//GUOYEE_PRT



	


#if (PRODUCT_TYPE==XIAMEN_HUIHENG_PRT)
#define		LEN_MODEL_VERSION		55
#define		LEN_MODEL				11		  
#define		MV_POS_MODEL			7
#define		MV_POS_HV				36
#define		MV_POS_SV				50
#define		BUILD_DATE_POS			(LEN_MODEL_VERSION+1)
#define		BUILD_TIME_POS			(BUILD_DATE_POS+12)
		  
#elif  (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)

#define		LEN_MODEL_VERSION		55
#define		LEN_MODEL				8
#define		MV_POS_MODEL			7
#define		MV_POS_HV				36
#define		MV_POS_SV				50
#define		BUILD_DATE_POS			(LEN_MODEL_VERSION+1)
#define		BUILD_TIME_POS			(BUILD_DATE_POS+12)

#else			// GUOYEE

#define		LEN_MODEL_VERSION		55
#define		LEN_MODEL				8//7
#define		MV_POS_MODEL			7
#define		MV_POS_HV				36
#define		MV_POS_SV				50
#define		BUILD_DATE_POS			(LEN_MODEL_VERSION+1)
#define		BUILD_TIME_POS			(BUILD_DATE_POS+12)

#endif


#endif

