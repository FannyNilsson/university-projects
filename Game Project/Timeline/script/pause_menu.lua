local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'

local InternalValues = InternalValues or require "script/lua/agentInternalData"

Project.PauseMenu = Project.PauseMenu or {}
PauseMenu = Project.PauseMenu

PauseMenu.custom_listener = PauseMenu.custom_listener or nil
PauseMenu.action = nil

local start_time = 0
local WwiseWorld = stingray.WwiseWorld
local button_sound_w = nil

local function perform_action()
	-- reload level
	if PauseMenu.action == "restart level" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
	    global_gameIsPaused_b = false
	    stingray.Application.set_time_step_policy("system_step_range",0,0.2)
		PauseMenu.shutdown()
		SimpleProject.change_level(levelNames[InternalValues.playerLoad.mapToLoad_i])
	-- go back to main menu
    elseif PauseMenu.action == "back to main menu" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
    
        global_gameIsPaused_b = false
        stingray.Application.set_time_step_policy("system_step_range",0,0.2)
		PauseMenu.shutdown()
		SimpleProject.change_level(Project.level_names.menu)
	-- shutdown pause menu and re-enter the game
    elseif PauseMenu.action == "continue" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        global_gameIsPaused_b = false
        stingray.Application.set_time_step_policy("system_step_range",0,0.2)
	    PauseMenu.shutdown()
	end
	PauseMenu.action = nil
	button_sound_w = nil
end

function PauseMenu.start()
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/pause_menu.s2d/pause_menu")
		--Register menu button mouse listener
		local custom_listener = PauseMenu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, PauseMenu.on_custom_event)
		PauseMenu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local back_to_main = function()
			PauseMenu.action = "back to main menu"
			perform_action()
		end
		local restart_level = function()
			PauseMenu.action = "restart level"
			perform_action()
		end
		
		local continue = function()
			PauseMenu.action = "continue"
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

	Appkit.manage_level_object(level, PauseMenu, nil)
	
	global_inPauseMenu_b = true
end

function PauseMenu.shutdown(object)
	if scaleform then
		scaleform.EventListener.disconnect(PauseMenu.custom_listener)
		scaleform.Stingray.unload_project()
		scaleform.Stingray.load_project_and_scene("content/ui/template_hud.s2d/template_hud")
        local insert_selected_hud_event = nil
        for i = 1, 4 do
            insert_selected_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "insert_selected",
                data = {selectedSlot = i, itemInSlot = global_inventoryManager_o.inventorySlots_t[i]}
            }
            scaleform.Stage.dispatch_event(insert_selected_hud_event)
        end
	end

	PauseMenu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, PauseMenu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
	global_inPauseMenu_b = false
end

function PauseMenu.on_custom_event(evt)
if evt.name == "action" then
		if evt.data.message == "continue" then
			PauseMenu.action = "continue"
		elseif evt.data.message == "back to main menu" then
			PauseMenu.action = "back to main menu"
		elseif evt.data.message == "restart level" then
		    PauseMenu.action = "restart level"
		end
	end
end

-- [[Main Menu custom functionality]]--
function PauseMenu.update(object, dt)
	if PauseMenu.debug_menu then
		PauseMenu.debug_menu:update()
	end
    
	if PauseMenu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					PauseMenu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					PauseMenu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				PauseMenu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				PauseMenu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

return PauseMenu
