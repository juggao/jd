/* calculate ISO date

    R.O.
    
    March 5 2021
    JD: 11
   
   
   KNOWN BUG:
   
   faketime '1900-01-01 08:25:30' ./iso
isodayyear: 1
isoweeknr: 53
isoweekday: 1
isoyear: 1899
 
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
    int year,month,day,i;
    unsigned long long daynumber,daynumber0,isoyear;
    struct iso_s ret;
    
    lt = time(0); 
    ptr = gmtime(&lt); 
    
    year = ptr->tm_year+1900;
    month = ptr->tm_mon+1;
    day = ptr->tm_mday;
    
    daynumber=gregdaynumber(year,month,day); 
    isoyear=floor(daynumber/365.2425)+2;
    
    for (i=0;i<4;i++) {
      daynumber0=isocalendardaynumber(isoyear-i,1,1);
      if (daynumber > daynumber0)
        break;
    }
    
    ret.isoyear=isoyear-i;
    ret.isoweeknr=floor((daynumber-daynumber0)/7)+1;
    ret.isoweekday=(daynumber-daynumber0)-7*(ret.isoweeknr-1)+1; 
    ret.isodayyear = ptr->tm_yday+1;
    ret.gregdaynumber = gregdaynumber(year,month,day);
    return ret;   
}

int main(void)
{
    struct iso_s s = iso();
    
    printf("isodayyear: %d\n", s.isodayyear);
    printf("isoweeknr: %d\n", s.isoweeknr);
    printf("isoweekday: %d\n", s.isoweekday);
    printf("isoyear: %d\n", s.isoyear);
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
