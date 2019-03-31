local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'

Project.CreditsMenu = Project.CreditsMenu or {}
CreditsMenu = Project.CreditsMenu

CreditsMenu.custom_listener = CreditsMenu.custom_listener or nil
CreditsMenu.action = nil

local start_time = 0
local WwiseWorld = stingray.WwiseWorld
local button_sound_w = nil

local function perform_action()
	-- go back to main menu
	if CreditsMenu.action == "back to main menu" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
		CreditsMenu.shutdown()
		MainMenu.start()
	    local temp_u = stingray.World.unit_by_name(stingray.Application.main_world(), "Geo_Blackboard")
	    stingray.Unit.set_data(temp_u, "mainCamera", true)
    end
	CreditsMenu.action = nil
end

function CreditsMenu.start()
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/credits_menu.s2d/credits_menu")
		--Register menu button mouse listener
		local custom_listener = CreditsMenu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, CreditsMenu.on_custom_event)
		CreditsMenu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local enter_game = function()
			CreditsMenu.action = "start"
			perform_action()
		end
		local exit = function()
			CreditsMenu.action = "exit"
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

	Appkit.manage_level_object(level, CreditsMenu, nil)
end

function CreditsMenu.shutdown(object)
	if scaleform then
		scaleform.EventListener.disconnect(CreditsMenu.custom_listener)
		scaleform.Stingray.unload_project()
	end

	CreditsMenu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, CreditsMenu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
	button_sound_w = nil
end

function CreditsMenu.on_custom_event(evt)
    if evt.name == "action" then
		if evt.data.message == "back to main menu" then
			CreditsMenu.action = "back to main menu"
		end
	end
end

-- [[Main Menu custom functionality]]--
function CreditsMenu.update(object, dt)
	if CreditsMenu.debug_menu then
		CreditsMenu.debug_menu:update()
	end

	if CreditsMenu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					CreditsMenu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					CreditsMenu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				CreditsMenu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				CreditsMenu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

return CreditsMenu
