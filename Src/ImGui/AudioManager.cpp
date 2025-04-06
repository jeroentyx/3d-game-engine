#include "AudioManager.h"

Eos::AudioManager::AudioManager()
{
    selectedBGMindex = 0;
    selectedSFXindex = 0;
    firstLaunch = true;
    isBGMTab = true;
}

void Eos::AudioManager::Update()
{
    if (firstLaunch)
    {
        BGMList = GET_AUDIO_SYSTEM->getBGMList();
        SFXList = GET_AUDIO_SYSTEM->getSFXList();
        currentlyPlaying = "Currently Playing: ";
        firstLaunch = false;
    }

    ImGui::Begin("Audio Manager", &active);
    
    ImGui::Text(currentlyPlaying.c_str());
    if (ImGui::Button("Play"))
    {
        if (isBGMTab)
        {
            if (selectedBGMindex > 0)
            {
                GET_AUDIO_SYSTEM->PlayTest(BGMList[selectedBGMindex]);
                UpdatePlayingString(BGMList[selectedBGMindex]);
            }
            else
            {
                UpdatePlayingString("");
            }
        }
        else
        {
            if (selectedSFXindex > 0)
            {
                GET_AUDIO_SYSTEM->PlayTest(SFXList[selectedSFXindex]);
                UpdatePlayingString(SFXList[selectedSFXindex]);
            }
            else
            {
                UpdatePlayingString("");
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        GET_AUDIO_SYSTEM->StopTest();
        UpdatePlayingString("");
    }
    ImGui::SameLine();
    if (ImGui::Button("Reload Audio Library"))
    {
        GET_AUDIO_SYSTEM->StopTest();
        ReloadAudio();
        selectedBGMindex = 0;
        selectedSFXindex = 0;
            
    }
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("BGM"))
        {
            ImGui::Text("Sample BGM Audio Here!");

            if (ImGui::BeginCombo(" ", BGMList[selectedBGMindex].c_str()))
            {
                for (int n = 0; n < BGMList.size(); n++)
                {
                    const bool is_selected = (selectedBGMindex == n);
                    if (ImGui::Selectable(BGMList[n].c_str(), is_selected))
                    {
                        selectedBGMindex = n;
                        isBGMTab = true;
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("SFX"))
        {
            ImGui::Text("Sample SFX Audio Here!");
            if (ImGui::BeginCombo(" ", SFXList[selectedSFXindex].c_str()))
            {
                for (int n = 0; n < SFXList.size(); n++)
                {
                    const bool is_selected = (selectedSFXindex == n);
                    if (ImGui::Selectable(SFXList[n].c_str(), is_selected))
                    {
                        selectedSFXindex = n;
                        isBGMTab = false;
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
    
}

void Eos::AudioManager::ReloadAudio()
{
    GET_AUDIO_SYSTEM->ReloadAllAudio();
    BGMList = GET_AUDIO_SYSTEM->getBGMList();
    SFXList = GET_AUDIO_SYSTEM->getSFXList();
}

void Eos::AudioManager::UpdatePlayingString(std::string songName)
{
    if (songName != "")
    {
        currentlyPlaying = "Currently Playing: " + songName;
    }
    else
    {
        currentlyPlaying = "Currently Playing: ";
    }
}
