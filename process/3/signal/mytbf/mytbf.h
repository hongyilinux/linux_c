#ifndef		MYTBF_H__
#define		MYTBF_H__

//上限
#define		MYTBF_MAX		1024
typedef		void mytbf_t;
//struct mytbf_st * mytbf_init(int cps,int burst);

mytbf_t *mytbf_init(int cps,int burst);

int mytbf_fetchtoken(mytbf_t * ,int);

int mytbf_returntoken(mytbf_t *,int);

int mytbf_destroy(mytbf_t *);







