#pragma once

int   GetRuns       (void);
float GetMusic      (void);
float GetSounds     (void);
void  SetMusic      (float s);
void  SetSounds     (float s);
void  ProfileRead   (void);
void  ProfileWrite  (void);
void  IncreaseRuns  (void);
void  SetFullScreen (bool c);
void  SetWindowSize (CP_Vector size);
void  SetColorMode  (ColorMode c);
void  ProfileReadControls(void);
void  ProfileWriteInputs(void);

ColorMode GetMode   (void);


