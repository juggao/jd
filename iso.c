/* calculate ISO date

    R.O.
    
    March 5 2021
    JD: 11
   
 
 */

#include<stdio.h>
#include<math.h>
#include<time.h>

struct iso_s
{
    unsigned long long isoyear;
    unsigned long long isoweeknr;
    unsigned long long isoweekday;
    unsigned long long isodayyear;
    unsigned long long gregdaynumber;
};

unsigned long long gregdaynumber(int year,int month, int day)
{
    unsigned long long y,m;
/* computes the day number since 0 January 0 CE (Gregorian) */
 
    y=year;
    m=month;
    if (month < 3) 
        y=y-1;
    if (month < 3) 
        m=m+12;
    return floor(365.25*y)-floor(y/100)+floor(y/400)+floor(30.6*(m+1))+day-62;
}


unsigned long long isocalendardaynumber(int isoyear,int isoweeknr, int isoweekday)
{
 
/* computes the day number since 0 January 0 CE (Gregorian) from the ISO 8601 calendar date */
    unsigned long long q,z,weeksum;
    
    q=floor(isoyear/400);
    z=isoyear-400*q;
    weeksum=20871*q+52*z+floor((5*z+7-4*floor((z-1)/100))/28)+isoweeknr;
    return 7*weeksum+isoweekday-5;
}


struct iso_s iso()
{
 
/* computes the ISO calendar date from the current Gregorian date */
    struct tm* ptr; 
    time_t lt; 
    int year,month,day,wday,i,weekday;
    unsigned long long daynumber,daynumber0,isoyear,isoweeknr,weekday0;
    long long d0,d;
    struct iso_s ret;
    
    lt = time(0); 
    ptr = gmtime(&lt); 
    
    year = ptr->tm_year+1900;
    month = ptr->tm_mon+1;
    day = ptr->tm_mday;
    wday = ptr->tm_wday;
    
    daynumber=gregdaynumber(year,month,day); 
    isoyear=floor(daynumber/365.2425)+2;
    
    weekday=((wday+6)%7)+1; /* weekdays will be numbered 1 to 7*/
    isoyear=year;
 
    d0=gregdaynumber(year,1,0);
    weekday0=((d0+4)%7)+1;
 
    d=gregdaynumber(year,month,day);
    isoweeknr=floor((d-d0+weekday0+6)/7)-floor((weekday0+3)/7);
 
/* check whether the last few days of December belong to the next year's ISO week */
 
    if((month == 11) && ((day-weekday) > 27)){
        isoweeknr=1;
        isoyear=isoyear+1;
    }
 
    /* check whether the first few days of January belong to the previous year's ISO week */
 
    if((month == 0) && ((weekday-day) > 3)){
        d0=gregdaynumber(year-1,1,0);
        weekday0=((d0+4)%7)+1;
        isoweeknr=floor((d-d0+weekday0+6)/7)-floor((weekday0+3)/7);
        isoyear=isoyear-1;
    }
 
   /* if(isoweeknr < 10) 
        return isoyear+"–W0"+isoweeknr+"–"+weekday;
    if(isoweeknr > 9) 
        return isoyear+"–W"+isoweeknr+"–"+weekday;
   */
    
    ret.isoyear=isoyear;
    ret.isoweeknr=isoweeknr;
    ret.isoweekday=weekday;
    ret.isodayyear = ptr->tm_yday+1;
    ret.gregdaynumber = gregdaynumber(year,month,day);
    return ret;   
}

int main(void)
{
    struct iso_s s = iso();
    
    printf("isodayyear: %lld\n", s.isodayyear);
    printf("isoweeknr: %lld\n", s.isoweeknr);
    printf("isoweekday: %lld\n", s.isoweekday);
    printf("isoyear: %lld\n", s.isoyear);
    /*printf("gregdaynumber: %d\n", s.gregdaynumber);*/
    return 0;
}



/* SOURCE:

https://webspace.science.uu.nl/~gent0113/calendar/isocalendar_text_5.htm

function gregdaynumber(year,month,day){
 
// computes the day number since 0 January 0 CE (Gregorian)
 
y=year;
m=month;
if(month < 3) y=y–1;
if(month < 3) m=m+12;
return Math.floor(365.25*y)–Math.floor(y/100)+Math.floor(y/400)+Math.floor(30.6*(m+1))+day–62;
}
 
function isocalendar1(){
 
// computes the ISO calendar date from the current Gregorian date
 
var today = new Date();
 
year=today.getFullYear();
month=today.getMonth(); // 0=January, 1=February, etc.
day=today.getDate();
wday=today.getDay();
 
weekday=((wday+6)%7)+1; // weekdays will be numbered 1 to 7
 
isoyear=year;
 
d0=gregdaynumber(year,1,0);
weekday0=((d0+4)%7)+1;
 
d=gregdaynumber(year,month+1,day);
isoweeknr=Math.floor((d–d0+weekday0+6)/7)–Math.floor((weekday0+3)/7);
 
// check whether the last few days of December belong to the next year's ISO week
 
if((month == 11) && ((day–weekday) > 27)){
  isoweeknr=1;
  isoyear=isoyear+1;
}
 
// check whether the first few days of January belong to the previous year's ISO week
 
if((month == 0) && ((weekday–day) > 3)){
  d0=gregdaynumber(year–1,1,0);
  weekday0=((d0+4)%7)+1;
  isoweeknr=Math.floor((d–d0+weekday0+6)/7)–Math.floor((weekday0+3)/7);
  isoyear=isoyear–1;
}
 
if(isoweeknr < 10) return isoyear+"–W0"+isoweeknr+"–"+weekday;
if(isoweeknr > 9) return isoyear+"–W"+isoweeknr+"–"+weekday;
}



function gregdaynumber(year,month,day){
 
// computes the day number since 0 January 0 CE (Gregorian)
 
y=year;
m=month;
if(month < 3) y=y–1;
if(month < 3) m=m+12;
return Math.floor(365.25*y)–Math.floor(y/100)+Math.floor(y/400)+Math.floor(30.6*(m+1))+day–62;
}
 
function isocalendardaynumber(isoyear,isoweeknr,isoweekday){
 
// computes the day number since 0 January 0 CE (Gregorian) from the ISO 8601 calendar date
 
q=Math.floor(isoyear/400);
z=isoyear–400*q;
weeksum=20871*q+52*z+Math.floor((5*z+7–4*Math.floor((z–1)/100))/28)+isoweeknr;
return 7*weeksum+isoweekday–5;
}

function isocalendar2(){
 
// computes the ISO calendar date from the current Gregorian date
 
var today = new Date();
 
year=today.getFullYear();
month=today.getMonth()+1; // 1=January, 2=February, etc.
day=today.getDate();
 
daynumber=gregdaynumber(year,month,day); 
 
isoyear=Math.floor(daynumber/365.2425)+2;
 
for(var i=0;i<3;i++){
  daynumber0=isocalendardaynumber(isoyear–i,1,1);
  if(daynumber > daynumber0){
    break;
  }
}
 
isoyear=isoyear–i;
isoweeknr=Math.floor((daynumber–daynumber0)/7)+1;
isoweekday=(daynumber–daynumber0)–7*(isoweeknr–1)+1; 
 
if(isoweeknr < 10) return isoyear+"–W0"+isoweeknr+"–"+isoweekday;
if(isoweeknr > 9) return isoyear+"–W"+isoweeknr+"–"+isoweekday;
}

*/
