/* calculate Janssen Day Number (JD = Julian Day number - 665101) 23/2/2021 

   R.O./VENLO  March 5 2021  
   
   JD=11


    EPOCH: [TONALPOHUALLI] 13-Miquiztli | 1-Ocelotl | 9-Calli
       [CE] 2021-02-23
       [LC] 13.0.8.5.6 
*/

#include<stdio.h>
#include<math.h>
#include<time.h>


double julianday_c( double day, double month, double year )
/*----------------------------------------------------------------------*/
/* Purpose: Convert a date in the Julian or Gregorian calendars into the*/
/*          corresponding Julian Day number (JD).                       */
/*          The Julian Day begins at Greenwich mean noon that is at 12h */
/*          Universal time (or 12h Ephemeris Time).                     */
/*          After 15 October 1582, the Gregorian Calendar is used.      */
/*----------------------------------------------------------------------*/
{
   double  A, B;
   double  JD;
   double  Gregorian = 1582.0 + 10.0/12.0 + 15.0/365.25;
 
  if (month < 3.0) { 
      year  -= 1.0; 
      month += 12.0; 
   }
   if ( (year + month/12.0 + day/365.25) >= Gregorian )  {		
      A = (double) ( (int)(year/100.0) );
      B = 2 - A + (double) ( (int)(A/4.0) );
   } else {
      B = 0;
   }
   if (year >= 0.0) {
      JD = (double) ( (int)( 365.25 * year ) ) + 
           (double) ( (int)( 30.6001 * (month + 1.0) ) ) +
           day + 1720994.5 + B;
   } else {
      JD = (double) ( (int)( 365.25 * year - 0.75 ) ) + 
           (double) ( (int)( 30.6001 * (month + 1.0) ) ) +
           day + 1720994.5 + B;                       
   }
   return( JD );
}

/* Compute parameters for a Gregorian month. */
unsigned long long jd_fluks(double day, double mon, double year)
{
    unsigned long long calday;
    double caldaycor;
    double cor100, cor400;
    
    calday = floor(30.6*((mon)+(((mon)<3)?13:1)))+floor(365.25*((1.*year)+(((mon)<3)?-1:0)));
    calday = calday+719164-719592-72744;

/* 719164, 719592 and 72744 correspond to shifts to 1/1/1970 [GC, Unix Time], 1/1/1 [JC], and March 1, 200 [GC/JC] 
 The calibration day being March 1, 1600
 calday has been computed for Day 0 and not Day 1 of each month */
 
    caldaycor = calday+1;
   
/* In 100 Julian years there are 75*365+25*366=100*365+25=36525 days.
 In 400 Julian years there are 4*36525+1=146100 days.
 calday(julian calendar 29/2/1600)=584094-72744
 Shift calday from Julian to Gregorian value: in 1600, cor100=0 and cor400=0
 the constant to be subtracted is 9 instead of 10 (= the shift itself). */
  
    cor100 = floor((((584094-72744)-caldaycor)/(36525)));
    cor400 = floor(((caldaycor-(584094-72744))/(146100)));
    calday = (calday-9)+cor100+cor400;
  
    return calday; /* first day = day of month */
}

/* Compute parameters for a Julian month. */
unsigned long long jd2_fluks(double day, double mon, double year)
{
    unsigned long long calday,firstday;
/* Compute the Day number. */
   calday = floor(30.6*((mon)+(((mon)<3)?13:1)))+floor(365.25*((1.*year)+(((mon)<3)?-1:0)));
   calday = calday+719164-719592-72744;
/* 719164, 719592 and 72744 correspond to shifts to 1/1/1970 [GC, Unix Time], 1/1/1 [JC], and March 1, 200 [GC/JC]  */
 
   return calday;
}



unsigned long long jd_day(unsigned long long calday, unsigned long long day)
{
    return (calday + day);
}
       
unsigned long long janssen_day(unsigned long long julianday)
{
    return (julianday - 665101);
}

long long dd_day(unsigned long long day)
{
    return (666666-day);
}

unsigned long long ro_day(unsigned long long calday)
{
    unsigned long long d = janssen_day(calday)-23;
    return d;
}

unsigned long long lp_day(unsigned long long calday)
{
    unsigned long long d = janssen_day(calday)-21;
    return d;
}

unsigned long long r4_day(unsigned long long calday)
{
    return (calday-665966);
}

int main(void)
{

    struct tm* ptr; 
    time_t lt; 
    unsigned long long juld,juld1,juld2,juld3;
    
    lt = time(NULL); 
    ptr = gmtime(&lt); 
    printf("UTC     : %s", asctime(ptr)); 
 
    printf("Day     : %d\n", ptr->tm_mday);
    printf("Month   : %d\n", ptr->tm_mon+1);
    printf("Year    : %d\n", ptr->tm_year+1900);
  
    juld1 = jd_fluks(ptr->tm_mday,ptr->tm_mon+1,ptr->tm_year+1900);
    juld2 = jd2_fluks(ptr->tm_mday,ptr->tm_mon+1,ptr->tm_year+1900);
    juld = jd_day(juld1,ptr->tm_mday);
    juld3 = jd_day(juld2,ptr->tm_mday);
    
    printf("Julian day (JDN): %f\n---\n", julianday_c(ptr->tm_mday,ptr->tm_mon+1,ptr->tm_year+1900)); 
    printf("Gregorian calendar: Chronological Julian day number since [JC/GC] March 1 200: %lld\n", juld);     
    /* printf("Gregorian calendar: First day month: %lld\n", juld1+1); */ /* +1 is the the first day of the month */
    printf("Julian calendar   : Chronological Julian day number since [JC/GC] March 1 200: %lld\n", juld3);     
    printf("Delta             : %lld\n---\n", juld3-juld);
    
    printf("Chronological Janssen day number (JD) since February 23 2021: %lld\n", janssen_day(juld));
    printf("Member of D66 from 18/3/2021 till 25/4/2021 \n");
    printf("Chronological Proyect day number: %lld\n", lp_day(juld)); 
    printf("Days since the fall of Rutte IV: %lld\n", r4_day(juld));
    printf("Days till DD-day (6-6-2025): %lld  Julian calendar: %lld\n---\n", dd_day(juld), dd_day(juld3));
      
    return 0;
}
