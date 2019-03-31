local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'

Project.WinMenu = Project.WinMenu or {}
WinMenu = Project.WinMenu

WinMenu.custom_listener = WinMenu.custom_listener or nil
WinMenu.action = nil

local start_time = 0
local WwiseWorld = stingray.WwiseWorld
local button_sound_w = nil

local function perform_action()
	-- Load credits menu
	if WinMenu.action == "credits" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        menu_music_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
	    WinMenu.shutdown()
	    CreditsMenu.start()
	end
	WinMenu.action = nil
end

function WinMenu.start()
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/win_menu.s2d/win_menu")
		--Register menu button mouse listener
		local custom_listener = WinMenu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, WinMenu.on_custom_event)
		WinMenu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local enter_credits = function()
			WinMenu.action = "credits"
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

	Appkit.manage_level_object(level, WinMenu, nil)
end

function WinMenu.shutdown(object)
	if scaleform then
		scaleform.EventListener.disconnect(WinMenu.custom_listener)
		scaleform.Stingray.unload_project()
	end

	WinMenu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, WinMenu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
end

function WinMenu.on_custom_event(evt)
    if evt.name == "action" then
		if evt.data.message == "credits" then
			WinMenu.action = "credits"
		end
	end
end

-- [[Main Menu custom functionality]]--
function WinMenu.update(object, dt)
	if WinMenu.debug_menu then
		WinMenu.debug_menu:update()
	end

	if WinMenu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					WinMenu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					WinMenu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				WinMenu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				WinMenu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

return WinMenu
