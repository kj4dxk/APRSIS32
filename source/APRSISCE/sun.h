#ifndef GOT_SUN_H
#define GOT_SUN_H

#ifdef __cplusplus
extern "C"
{
#endif

int SunRiseSet(double latitude, double longitude, int year, int month, int day, double *zHoursRise, double *zHoursSet);

#ifdef __cplusplus
}
#endif

#endif	/* GOT_SUN_H */

