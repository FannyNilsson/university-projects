local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'

Project.loadNote_menu = Project.loadNote_menu or {}
loadNote_menu = Project.loadNote_menu

loadNote_menu.custom_listener = loadNote_menu.custom_listener or nil
loadNote_menu.action = nil

local start_time = 0
local test_inv 
local WwiseWorld = stingray.WwiseWorld
local button_sound_w = nil

local function perform_action()
	-- Load empty level
	if loadNote_menu.action == "back to main menu" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        menu_music_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
		loadNote_menu.shutdown()
		--SimpleProject.change_level(Project.level_names)
		MainMenu.start()
	end
	loadNote_menu.action = nil
	button_sound_w = nil
end

function loadNote_menu.start()
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/loadNote.s2d/loadNote")
		--Register menu button mouse listener
		local custom_listener = loadNote_menu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, loadNote_menu.on_custom_event)
		loadNote_menu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local closePopUp = function()
			loadNote_menu.action = "back to main menu"
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

	Appkit.manage_level_object(level, loadNote_menu, nil)
end

function loadNote_menu.shutdown(object)
	if scaleform then
		scaleform.EventListener.disconnect(loadNote_menu.custom_listener)
		scaleform.Stingray.unload_project()
	end

	loadNote_menu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, loadNote_menu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
end

--registers the custom event and sets the action variable
function loadNote_menu.on_custom_event(evt)
    if evt.name == "action" then
		if evt.data.message == "back to main menu" then
			loadNote_menu.action = "back to main menu"
		end
	end
end

-- [[Main Menu custom functionality]]--
function loadNote_menu.update(object, dt)
	if loadNote_menu.debug_menu then
		loadNote_menu.debug_menu:update()
	end

	if loadNote_menu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					loadNote_menu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					loadNote_menu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				loadNote_menu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				loadNote_menu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

return loadNote_menu
