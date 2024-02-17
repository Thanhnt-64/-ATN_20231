#ifndef MAX30100_FILTERS_H
#define MAX30100_FILTERS_H

// http://www.schwietering.com/jayduino/filtuino/
// Low pass butterworth filter order=1 alpha1=0.1
// Fs=100Hz, Fc=6Hz
static float v[2];
struct FilterBuLp1{
    void (*FilterBuLp1)(void);
    float (*step)(float x);
};

void FilterBuLp1(void){
    v[0] = 0.0;
}

float step(float x){
    v[0] = v[1];
    v[1] = (2.452372752527856026e-1 * x) + (0.50952544949442879485 * v[0]);
    return (v[0] + v[1]);
}
extern const struct FilterBuLp1 lpf = {
    FilterBuLp1,
    step
};
// http://sam-koblenski.blogspot.de/2015/11/everyday-dsp-for-programmers-dc-and.html

static float alpha;
static float dcw;

static float redAlpha;
static float redDCW;

struct DCRemover
{
    void (*DCRemover)(void);
    void (*DCRemover2)(float alpha_);
    float (*step)(float x);
    float (*getDCW)(void);
};
/* ir DC Remover-------------------------------------*/
void irDCRemover(void){
    alpha = 0;
    dcw = 0;
}
void irDCRemover2(float alpha_){
    alpha = alpha_;
    dcw = 0;
}
float irStep(float x){
    float olddcw = dcw;
    dcw = (float)x + alpha * dcw;
    return dcw - olddcw;
}
float irGetDCW(void){
    return dcw;
}

extern const struct DCRemover irDCRemover = {
    irDCRemover,
    irDCRemover2,
    irStep,
    irGetDCW
}


/* red DC Remover------------------------------------*/
void redDCRemover(void){
    redAlpha = 0;
    redDCW = 0;
}
void redDCRemover2(float alpha_){
    redAlpha = alpha_;
    redDCW = 0;
}
float redStep(float x){
    float olddcw = redDCW;
    redDCW = (float)x + redAlpha * redDCW;
    return redDCW - olddcw;
}
float redGetDCW(void){
    return redDCW;
}

extern const struct DCRemover redDCRemover = {
    redDCRemover,
    redDCRemover2,
    redStep,
    redGetDCW
}
#endif
