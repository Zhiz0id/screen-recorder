#ifndef VNC2MPG_H_INCLUDED
#define VNC2MPG_H_INCLUDED

#ifdef __cplusplus
   extern "C" {
#endif

extern "C" void cmain(const char *filename_const, int *stop, int *scale, 
                      int *fps, int *bps, int *codec);

#ifdef __cplusplus
   }
#endif

#endif /* VNC2MPG_H_INCLUDED */
