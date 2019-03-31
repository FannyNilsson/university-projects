local InternalValues = InternalValues or require "script/lua/agentInternalData"																			   
require 'script/lua/player'

	

local foundSaveFile_st = stingray.SaveSystem.auto_load("save_file")
local loadedGame_st
local saveGame_t = {inv, position_v3, rotation_v3, currentLevel_i}
local loadSettings_st 
local loadPlayerSettings_st

--loads the game using stingrays built-in SaveSystem
function LoadGame()
    loadedGame_st = stingray.SaveSystem.auto_load("save_file")
end--end of LoadGame()

--saves the inventory, player position and rotation and the previous level in a file called save_file
function SaveGame(inv1, pos, rot, lvl)
    global_foundSavedFile_b = true

    saveGame_t = {inv = inv1, position_v3 = pos, rotation_v3 = rot, currentLevel_i = lvl}
    local saveGame_st = stingray.SaveSystem.auto_save("save_file", saveGame_t)
    stingray.SaveSystem.close(saveGame_st)
    SetInitData(lvl, pos, rot, inv1)
end--end of SaveGame()

--is called in update in project. This checks if any file has been saved and catches errors if no file have been found. It also makes sure that the file we are loading from is done loading
function LoadUpdate()
    if(loadedGame_st ~= nil) then
        local progress = stingray.SaveSystem.progress(loadedGame_st)
	    if(progress.done) then
	        if(progress.error ~= nil) then
	            stingray.SaveSystem.close(loadedGame_st)
	            loadedGame_st = nil
	            return
            end
            global_currentLevel_f = progress.data.currentLevel_i
	        InternalValues.playerLoad.mapToLoad_i = progress.data.currentLevel_i
	        InternalValues.playerLoad.spawnPos_v3b:store(progress.data.position_v3)
	        InternalValues.playerLoad.spawnRot_v3b:store(progress.data.rotation_v3)
	        InternalValues.playerLoad.inventorySlot1_s = progress.data.inv.inventorySlots_t[1]
	        InternalValues.playerLoad.inventorySlot2_s = progress.data.inv.inventorySlots_t[2]
	        InternalValues.playerLoad.inventorySlot3_s = progress.data.inv.inventorySlots_t[3]
	        InternalValues.playerLoad.inventorySlot4_s = progress.data.inv.inventorySlots_t[4]
	        
	        InternalValues.playerLoad.itemSubTypeSlot1_i = progress.data.inv.itemSubTypes_t[1]
	        InternalValues.playerLoad.itemSubTypeSlot2_i = progress.data.inv.itemSubTypes_t[2]
	        InternalValues.playerLoad.itemSubTypeSlot3_i = progress.data.inv.itemSubTypes_t[3]
	        InternalValues.playerLoad.itemSubTypeSlot4_i = progress.data.inv.itemSubTypes_t[4]
	        
	        stingray.SaveSystem.close(loadedGame_st)
	        loadedGame_st = nil
	        SimpleProject.change_level(levelNames[InternalValues.playerLoad.mapToLoad_i])
        end
    end
end--end of LoadUpdate()

function CheckSaveFile()
    if(foundSaveFile_st ~= nil) then
        local progress = stingray.SaveSystem.progress(foundSaveFile_st)
	    if(progress.done) then
	        if(progress.error == nil) then
	            global_foundSavedFile_b = true
            end
            stingray.SaveSystem.close(foundSaveFile_st)
            foundSaveFile_st = nil
        end
    end
end--end of CheckSaveFile()

--save function for all settings - sound and key bindings
function SaveSettings()
    local settings_t = InternalValues.settings
    local saveSettings_st = stingray.SaveSystem.auto_save("settings_file", settings_t)
    stingray.SaveSystem.close(saveSettings_st)
    
    local settingsStr_t = InternalValues.saveDataStrings
    local saveSettings_st = stingray.SaveSystem.auto_save("playersettings_file", settingsStr_t)
    stingray.SaveSystem.close(saveSettings_st)
    
end--end of SaveSettings()

--loads the settings if a saved file exists
function LoadSettings()
    loadSettings_st = stingray.SaveSystem.auto_load("settings_file")
    loadPlayerSettings_st = stingray.SaveSystem.auto_load("playersettings_file")
end--end of LoadSettings()

--Checks if a saved file for settings exists and makes sure that the loading is complete before it reads the data and sets the right variables
function LoadSettingsUpdate()
    local sound_W = stingray.Wwise.wwise_world(stingray.Application.main_world())
    if(loadSettings_st ~= nil) then
        local progress = stingray.SaveSystem.progress(loadSettings_st)
	    if(progress.done) then
	        if(progress.error ~= nil) then
	            stingray.SaveSystem.close(loadSettings_st)
	            loadSettings_st = nil
	            return
            end
	        InternalValues.settings.volume_f = progress.data.volume_f
	        stingray.WwiseWorld.set_global_parameter(sound_W, "CODE_Volume_SFX", progress.data.volume_f)
	        stingray.WwiseWorld.set_global_parameter(sound_W, "CODE_Volume_Music", progress.data.volume_f)
	        stingray.WwiseWorld.set_global_parameter(sound_W, "CODE_Volume_VO", progress.data.volume_f)
	        
	        
	        InternalValues.settings.mouseSens_f = progress.data.mouseSens_f
	        
	        InternalValues.settings.jump_f = progress.data.jump_f
	        InternalValues.settings.run_f = progress.data.run_f
	        InternalValues.settings.crouch_f = progress.data.crouch_f
	        InternalValues.settings.left_f = progress.data.left_f
	        InternalValues.settings.right_f = progress.data.right_f
	        InternalValues.settings.forward_f = progress.data.forward_f
	        InternalValues.settings.backwards_f = progress.data.backwards_f
	        InternalValues.settings.timeshift_f = progress.data.timeshift_f
	        InternalValues.settings.interact_f = progress.data.interact_f
	        InternalValues.settings.drop_f = progress.data.drop_f
	        InternalValues.settings.throw_f = progress.data.throw_f
	        
	        stingray.SaveSystem.close(loadSettings_st)
	        loadSettings_st = nil
        end
    end
--loads the key bindings types (keyboard or mouse) and sets the variables 
    if(loadPlayerSettings_st ~= nil) then
        local progress = stingray.SaveSystem.progress(loadPlayerSettings_st)
	    if(progress.done) then
	        if(progress.error ~= nil) then
	            stingray.SaveSystem.close(loadPlayerSettings_st)
	            loadPlayerSettings_st = nil
	            return
            end
	        
	        local tempPlayerSettings_t = {}
            for key, value in pairs(progress.data) do
                if(value == "Mouse") then
                    InternalValues.playerSettings.key = stingray.Mouse
                elseif(value == "Keyboard") then 
                    InternalValues.playerSettings.key = stingray.Keyboard
                end
            end
	        
	        stingray.SaveSystem.close(loadPlayerSettings_st)
	        loadPlayerSettings_st = nil
        end
    end
end--end of LoadSettingsUpdate()

function LevelReset()

    SimpleProject.change_level(levelNames[4])
end
LoadSettings()