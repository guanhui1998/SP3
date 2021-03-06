#pragma once

#ifndef MAP_EDITOR_EXPORTS
#define MAP_EDITOR_API __declspec(dllexport) 
#else
#define MAP_EDITOR_API __declspec(dllimport) 
#endif

#include "Platform.h"
#include <vector>
#include <string>

class MAP_EDITOR_API Map_Editor
{
public:
    Map_Editor();
    ~Map_Editor();

    enum EDIT_STATE
    {
        START,
        SAVE,
        LOAD,
        CREATE,
        MANAGE,
        DESTROY,
        END,
    };



    EDIT_STATE edit_state;

    void Init(Vector3 topofScreen);
    void Update(float dt, Vector3 mousepos);
    void PlatformHandler(Platform* selected_platform, float dt);
    std::string getState();
    static bool IsKeyPressed(unsigned short key);
    Platform* CreateNewPlatform(Vector3 pos, Vector3 scale, Platform::PLATFORM_TYPE type);
    Platform* CreatePlatform(Vector3 pos, Vector3 scale, Platform::PLATFORM_TYPE type);
    std::string TextForDisplay();
    std::string getfilename(std::string fileDir);
    void DisplayAvailablePlatforms(Vector3 topofScreen);
    void MouseOverCreatePlatform();
    void MouseOverSelectPlatform();
    void DeleteSelected();
    void PushFilenamesIntoStringVec();
    void FileSelection();
    void SaveToFile(std::string file);
    void LoadFromFile(std::string file);
    std::string getSelectedFile();

    std::vector<std::vector<int>> Tilemap;
    std::vector<Platform* > Platform_List;
    std::vector<Vector3> EnemySpawn_List;
    std::vector<Platform* > Platform_Display_List;
    std::vector<std::string> SaveList;
    Platform* curr;
    Platform* nullplat;
private:
    Vector3 mousepos;
    Vector3 playerspawn;
    float inputDelayTimer;
    std::string Fileselect;
    bool pushedfilenames;

    int row, col, tilesq;
};

MAP_EDITOR_API Map_Editor* CreateNewMapEditorInstance();