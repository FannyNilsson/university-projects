local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'
local InternalValues = InternalValues or require "script/lua/agentInternalData"
OptionsMenu = require 'script/lua/options_menu'
CreditsMenu = require 'script/lua/credits_menu'
PauseMenu = require 'script/lua/pause_menu'
LevelCompletedMenu = require 'script/lua/level_completed_menu'
WinMenu = require 'script/lua/win_menu'
loadNote = require 'script/lua/loadNote_menu'
startWarningMenu = require 'script/lua/startWarning_menu'
KeyBindingsMenu = require 'script/lua/key_bindings_menu'
GraphicsMenu = require 'script/lua/graphics_menu'
require 'script/lua/load_save'

Project.MainMenu = Project.MainMenu or {}
MainMenu = Project.MainMenu

MainMenu.custom_listener = MainMenu.custom_listener or nil
MainMenu.action = nil

local start_time = 0
global_foundSavedFile_b = false

local Wwise = stingray.Wwise
local WwiseWorld = stingray.WwiseWorld
local world_w = stingray.Application.main_world()
global_menu_music_w = nil
local button_sound_w = nil


local function perform_action()
    if(global_menu_music_w == nil) then
        if Wwise then
            if(not global_winMusic_b) then
                wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                global_menu_music_w = WwiseWorld.trigger_event(wwise_world, "music_menu")
            else
                wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                global_menu_music_w = WwiseWorld.trigger_event(wwise_world, "music_credits")
            end
        elseif(global_menu_music_w ~= nil) then
            if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()), global_menu_music_w)) then
                if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()), global_menu_music_w)) then
                    if Wwise then
                        if(not global_winMusic_b) then
                            wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                            global_menu_music_w = WwiseWorld.trigger_event(wwise_world, "music_menu")
                        else
                            wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                            global_menu_music_w = WwiseWorld.trigger_event(wwise_world, "music_credits")
                        end
                    end
                end
            end
        end
    end
	-- Load empty level but if there is a saved file a warning will pop up telling the user that he/she will overwrite the data if they proceed
	if MainMenu.action == "start" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
	    MainMenu.shutdown()
	    if(global_foundSavedFile_b) then
	        startWarningMenu.start()
        else
		    SimpleProject.change_level("content/_Assets/Levels/Intro_Dunkelberg_Level")
		    ResetInitData()
		    local tempTable = InternalValues.playerLoad
	        SaveGame(global_inventoryManager_o,stingray.Vector3Box.unbox(tempTable.spawnPos_v3b), stingray.Vector3Box.unbox(tempTable.spawnRot_v3b), tempTable.mapToLoad_i)
	    end
	-- Exit the program
    elseif MainMenu.action == "exit" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")

		stingray.Application.quit()
	--load options menu
    elseif MainMenu.action == "options" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
    
	    MainMenu.shutdown()
	    OptionsMenu.start()
	--load credits menu
    elseif MainMenu.action == "credits" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
    
	    MainMenu.shutdown()
	    CreditsMenu.start()
	    local temp_u = stingray.World.unit_by_name(stingray.Application.main_world(), "Geo_Blackboard")
	    stingray.Unit.set_data(temp_u, "mainCamera", false)
	--load saved game. If no saved game exists a note will pop up
    elseif MainMenu.action == "load game" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        MainMenu.shutdown()
        if(not global_foundSavedFile_b) then
            loadNote.start()
        else
            LoadGame()
        end
	end
	MainMenu.action = nil
	button_sound_w = nil
end

function MainMenu.start()
    global_playedDungeonCutscene = false
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/simple_menu.s2d/simple_menu")
		--Register menu button mouse listener
		local custom_listener = MainMenu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, MainMenu.on_custom_event)
		MainMenu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local enter_game = function()
			MainMenu.action = "start"
			perform_action()
		end
		local exit = function()
			MainMenu.action = "exit"
			perform_action()
		end
		
		local options = function()
			MainMenu.action = "options"
			perform_action()
		end
		
		local credits = function()
			MainMenu.action = "credits"
			perform_action()
		end
		
		local load_game = function()
			MainMenu.action = "load game"
			perform_action()
		end
	end

	local level = SimpleProject.level
	start_time = stingray.World.time(SimpleProject.world)

	-- make sure camera is at correct location
	local camera_unit = SimpleProject.camera_unit
	local camera = stingray.Unit.camera(camera_unit, 1)
	stingray.Unit.set_local_pose(camera_unit, 1, stingray.Matrix4x4.identity())
	stingray.Camera.set_local_pose(camera, camera_unit, stingray.Matrix4x4.identity())

	Appkit.manage_level_object(level, MainMenu, nil)
end

function MainMenu.shutdown(object)
	if scaleform then
		scaleform.EventListener.disconnect(MainMenu.custom_listener)
		scaleform.Stingray.unload_project()
	end

	MainMenu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, MainMenu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
end

--registers the custom event and sets the action variable
function MainMenu.on_custom_event(evt)
    if evt.name == "action" then
		if evt.data.message == "start" then
			MainMenu.action = "start"
		elseif evt.data.message == "exit" then
			MainMenu.action = "exit"
		elseif evt.data.message == "options" then
		    MainMenu.action = "options"
		elseif evt.data.message == "credits" then
		    MainMenu.action = "credits"
		elseif evt.data.message == "load game" then
		    MainMenu.action = "load game"
		end
	end
end

-- [[Main Menu custom functionality]]--
function MainMenu.update(object, dt)
	if MainMenu.debug_menu then
		MainMenu.debug_menu:update()
	end

	if MainMenu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					MainMenu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					MainMenu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				MainMenu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				MainMenu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

return MainMenu
