local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'
local InternalValues = InternalValues or require "script/lua/agentInternalData"

Project.startWarningMenu = Project.startWarningMenu or {}
startWarningMenu = Project.startWarningMenu

startWarningMenu.custom_listener = startWarningMenu.custom_listener or nil
startWarningMenu.action = nil

local start_time = 0
local WwiseWorld = stingray.WwiseWorld
local button_sound_w = nil

local function perform_action()
	-- Load a new game
	if startWarningMenu.action == "start new game" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    ResetInitData()
	    local tempTable = InternalValues.playerLoad
	    SaveGame(global_inventoryManager_o,stingray.Vector3Box.unbox(tempTable.spawnPos_v3b), stingray.Vector3Box.unbox(tempTable.spawnRot_v3b), tempTable.mapToLoad_i)
	    startWarningMenu.shutdown()
	    SimpleProject.change_level(Project.level_names.level_0)
	-- Go back to main menu
    elseif startWarningMenu.action == "back to main menu" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
    
		startWarningMenu.shutdown()
		MainMenu.start()
	end
	startWarningMenu.action = nil
	button_sound_w = nil
end

function startWarningMenu.start()
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/startWarning_menu.s2d/startWarning_menu")
		--Register menu button mouse listener
		local custom_listener = startWarningMenu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, startWarningMenu.on_custom_event)
		startWarningMenu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local start_new_game = function()
			startWarningMenu.action = "start new game"
			perform_action()
		end
		local back_to_main_menu = function()
			startWarningMenu.action = "back to main menu"
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

	Appkit.manage_level_object(level, startWarningMenu, nil)
end

function startWarningMenu.shutdown(object)
    if WWise then
        if(stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()), global_menu_music_w)) then
            wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
            WwiseWorld.stop_event(stingray.Wwise.wwise_world(stingray.Application.main_world()), global_menu_music_w)
            global_menu_music_w = nil
        end
    end
    
	if scaleform then
		scaleform.EventListener.disconnect(startWarningMenu.custom_listener)
		scaleform.Stingray.unload_project()
	end

	startWarningMenu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, startWarningMenu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
end

--registers the custom event and sets the action variable
function startWarningMenu.on_custom_event(evt)
    if evt.name == "action" then
		if evt.data.message == "start new game" then
			startWarningMenu.action = "start new game"
		elseif evt.data.message == "back to main menu" then
			startWarningMenu.action = "back to main menu"
		end
	end
end

-- [[Main Menu custom functionality]]--
function startWarningMenu.update(object, dt)
	if startWarningMenu.debug_menu then
		startWarningMenu.debug_menu:update()
	end

	if startWarningMenu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					startWarningMenu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					startWarningMenu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				startWarningMenu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				startWarningMenu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

return startWarningMenu
