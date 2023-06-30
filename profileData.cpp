#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include "playerInput.h"
extern "C"
{


#include "structs.h"
#include "globalData.h"
#include <ShlObj_core.h>

  struct profileData
  {
    ColorMode mode;
    int runs;
    float SoundsVolume;
    float MusicVolume;
    bool fullScreen;
    CP_Vector windowSize;
  };


  struct profileData profile = { LightMode, 0, 1, 1, false, {1920, 1080} };

  void SetWindowSize(CP_Vector size) 
  {
    profile.windowSize = size;
  }

  void IncreaseRuns(void) 
  {
    profile.runs++;
  }

  int GetRuns(void) 
  {
    return profile.runs;
  }

  void SetColorMode(ColorMode c) 
  {
    profile.mode = c;
  }

  ColorMode GetMode(void) 
  {
    return profile.mode;
  }

  float GetSounds(void) 
  {
    return profile.SoundsVolume;
  }

  void SetSounds(float s) 
  {
    profile.SoundsVolume = s;
  }

  float GetMusic(void) 
  { 
    return profile.MusicVolume; 
  }

  void SetMusic(float s) 
  {
    profile.MusicVolume = s;
  }

  void SetFullScreen(bool c) 
  {
    profile.fullScreen = c;
  }

  void ProfileRead(void)
  {
    wchar_t* p;
    std::wstring path;
    SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &p);

    std::wstringstream ss;
    ss << p << L"\\";

    CoTaskMemFree(static_cast<void*>(p));
    std::wstring string_to_convert = ss.str();

    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string converted_str = converter.to_bytes(string_to_convert);
    converted_str += "DescentGames\\";
    converted_str += "Simply_Sruvive\\";
    converted_str += "Options.dat";

    std::ifstream read;
    read.open(converted_str, std::ios_base::in);
    std::string token;

    if (read.is_open() == false)
      goto end;
    while (!read.eof()) 
    {
      std::getline(read, token);
      if (token.find("ColorMode : ") != std::string::npos) 
      {
        const int len = static_cast<int>(std::string("ColorMode : ").length());
        std::string sub = token.substr(token.find("ColorMode : ") + len, token.length() - len);
        if (sub == "LightMode")
          setColorMode(LightMode);
        else if (sub == "DarkMode")
          setColorMode(DarkMode);
        continue;
      }

      if (token.find("Runs : ") != std::string::npos)
      {
        const int len = static_cast<int>(std::string("Runs : ").length());
        std::string sub = token.substr(token.find("Runs : ") + len, token.length() - len);
        profile.runs = atoi(sub.c_str());
        continue;
      }
      if (token.find("Sound : ") != std::string::npos)
      {
        const int len = static_cast<int>(std::string("Sounds : ").length());
        std::string sub = token.substr(token.find("Sounds : ") + len, token.length() - len);
        profile.SoundsVolume = (float)atof(sub.c_str());
        CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, profile.SoundsVolume);

        continue;
      }

      if (token.find("Music : ") != std::string::npos)
      {
        const int len = static_cast<int>(std::string("Music : ").length());
        std::string sub = token.substr(token.find("Music : ") + len, token.length() - len);
        profile.MusicVolume = (float)atof(sub.c_str());
        CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, profile.MusicVolume);

        continue;
      }

      if (token.find("SizeX : ") != std::string::npos)
      {
        const int len = static_cast<int>(std::string("SizeX : ").length());
        std::string sub = token.substr(token.find("SizeX : ") + len, token.length() - len);
        profile.windowSize.x = (float)atof(sub.c_str());
        continue;
      }

      if (token.find("SizeY : ") != std::string::npos)
      {
        const int len = static_cast<int>(std::string("SizeY : ").length());
        std::string sub = token.substr(token.find("SizeY : ") + len, token.length() - len);
        profile.windowSize.y = (float)atof(sub.c_str());
        continue;
      }
      
      if (token.find("FullScreen : ") != std::string::npos) 
      {
      
        const int len = static_cast<int>(std::string("FullScreen : ").length());
        std::string sub = token.substr(token.find("FullScreen : ") + len, token.length() - len);
        if (sub == "False")
          profile.fullScreen = false;
        else if (sub == "True")
          profile.fullScreen = true;
        continue;
      }


    }

    end:
    if (read.is_open())
      read.close();
    if(profile.fullScreen)
      CP_System_FullscreenAdvanced(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
    else if(profile.windowSize.x != 0 && profile.windowSize.y != 0)
      CP_System_SetWindowSize((int)profile.windowSize.x, (int)profile.windowSize.y);
    else
      CP_System_SetWindowSize(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());

  }


  void ProfileWrite(void)
  {
    wchar_t* p;
    std::wstring path;
    SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &p);

    std::wstringstream ss;
    ss << p << L"\\";

    CoTaskMemFree(static_cast<void*>(p));
    std::wstring string_to_convert = ss.str();

    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string converted_str = converter.to_bytes(string_to_convert);
    converted_str += "DescentGames\\";

    CreateDirectory(converted_str.c_str(), NULL);

    converted_str += "Simply_Sruvive\\";
    CreateDirectory(converted_str.c_str(), NULL);
    converted_str += "Options.dat";

    std::ofstream write;
    write.open(converted_str, std::ios::out);

    write << "ColorMode : " << ((profile.mode == LightMode) ? "LightMode" : "DarkMode") << "\n";
    write << "Runs : " << profile.runs << "\n";
    write << "Sound : " << profile.SoundsVolume << "\n";
    write << "Music : " << profile.MusicVolume << "\n";
    write << "FullScreen : " << ((profile.fullScreen == true) ? "True" : "False") << "\n";
    write << "SizeX : " << profile.windowSize.x << "\n";
    write << "SizeY : " << profile.windowSize.y << "\n";

    write << std::endl;
    if (write.is_open())
      write.close();
  }
  void ProfileWriteInputs(void);
  void SetDefaultInputs(void)
  {
    addBinding(Shoot, tMouse, bindings(MOUSE_BUTTON_1) );
    addBinding(Confirm, tMouse, bindings(MOUSE_BUTTON_1));

    addBinding(Shoot,     Key, bindings(KEY_SPACE));
    addBinding(Inventory, Key, bindings(KEY_E));
    addBinding(Reload,    Key, bindings(KEY_R));
    addBinding(Wheel,     Key, bindings(KEY_Q));
    addBinding(Dash,      Key, bindings(KEY_LEFT_CONTROL));
    addBinding(Dash,      Key, bindings(KEY_LEFT_SHIFT));
    addBinding(Confirm,   Key, bindings(KEY_ENTER));
    addBinding(Return,    Key, bindings(KEY_ESCAPE));
    addAxisBinding(false, true,  Key, bindings(KEY_W));
    addAxisBinding(false, false, Key, bindings(KEY_S));
    addAxisBinding(true,  true,  Key, bindings(KEY_D));
    addAxisBinding(true,  false, Key, bindings(KEY_A));
    
    addBinding(Shoot,     Controller, bindings(GAMEPAD_LEFT_SHOULDER));
    addBinding(Inventory, Controller, bindings(GAMEPAD_BACK));
    addBinding(Reload,    Controller, bindings(GAMEPAD_X));
    addBinding(Confirm,   Controller, bindings(GAMEPAD_A));
    addBinding(Back,      Controller, bindings(GAMEPAD_B));
    addBinding(Return,    Controller, bindings(GAMEPAD_START));
    addBinding(Wheel,     Controller, bindings(GAMEPAD_Y));
    addBinding(XAxisPos,  Controller, bindings(GAMEPAD_DPAD_RIGHT));
    addBinding(XAxisNeg,  Controller, bindings(GAMEPAD_DPAD_LEFT));
    addBinding(YAxisPos,  Controller, bindings(GAMEPAD_DPAD_UP));
    addBinding(YAxisNeg,  Controller, bindings(GAMEPAD_DPAD_DOWN));
    addAxisBinding(false, true,  Controller, bindings(GAMEPAD_DPAD_UP));
    addAxisBinding(false, false, Controller, bindings(GAMEPAD_DPAD_DOWN));
    addAxisBinding(true,  true,  Controller, bindings(GAMEPAD_DPAD_RIGHT));
    addAxisBinding(true,  false, Controller, bindings(GAMEPAD_DPAD_LEFT));




  }


  void ProfileReadControls(void) 
  {
    wchar_t* p;
    std::wstring path;
    SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &p);

    std::wstringstream ss;
    ss << p << L"\\";

    CoTaskMemFree(static_cast<void*>(p));
    std::wstring string_to_convert = ss.str();

    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string converted_str = converter.to_bytes(string_to_convert);
    converted_str += "DescentGames\\";
    converted_str += "Simply_Sruvive\\";
    converted_str += "Controls.ini";

    std::ifstream read;
    read.open(converted_str, std::ios_base::in);
    std::string token;
    if (read.is_open() == false)
      SetDefaultInputs();




  }

  void ProfileWriteInputs(void) 
  {
    wchar_t* p;
    std::wstring path;
    SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &p);

    std::wstringstream ss;
    ss << p << L"\\";

    CoTaskMemFree(static_cast<void*>(p));
    std::wstring string_to_convert = ss.str();

    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string converted_str = converter.to_bytes(string_to_convert);
    converted_str += "DescentGames\\";

    CreateDirectory(converted_str.c_str(), NULL);

    converted_str += "Simply_Sruvive\\";
    CreateDirectory(converted_str.c_str(), NULL);
    converted_str += "Controls.ini";
  
  }


}
