local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'
local internalValues = require 'script/lua/agentInternalData'
require 'script/lua/load_save'

Project.OptionsMenu = Project.OptionsMenu or {}
local OptionsMenu = Project.OptionsMenu

OptionsMenu.custom_listener = OptionsMenu.custom_listener or nil
OptionsMenu.action = nil

local sound_W = stingray.Wwise.wwise_world(stingray.Application.main_world())
local start_time = 0
local volume_f
local mouseSens_f

local WwiseWorld = stingray.WwiseWorld
local button_sound_w = nil

local function perform_action()
	if OptionsMenu.action == "volume" then
	    
	    internalValues.settings.volume_f = volume_f
	    stingray.WwiseWorld.set_global_parameter(sound_W, "CODE_Volume_SFX", volume_f)
	    stingray.WwiseWorld.set_global_parameter(sound_W, "CODE_Volume_Music", volume_f)
	    stingray.WwiseWorld.set_global_parameter(sound_W, "CODE_Volume_VO", volume_f)
	    
	elseif OptionsMenu.action == "key bindings" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
	    OptionsMenu.shutdown() 
	    KeyBindingsMenu.start()
	elseif OptionsMenu.action == "graphics settings" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
	    OptionsMenu.shutdown() 
	    GraphicsMenu.start()
	elseif OptionsMenu.action == "back to main menu" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
	    SaveSettings()
	    OptionsMenu.shutdown()
	    MainMenu.start()
    elseif OptionsMenu.action == "mouse sensitivity" then
        
        if(mouseSens_f == 0) then
            internalValues.settings.mouseSens_f = 0.001
        else
            internalValues.settings.mouseSens_f = mouseSens_f * 0.001
        end
        print(internalValues.settings.mouseSens_f)
	end
	OptionsMenu.action = nil
	button_sound_w = nil
end

function OptionsMenu.start()
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end
	
	
-- 	wwise_world = wwise_world or stingray.Wwise.wwise_world(SimpleProject.world)
-- 	stingray.WwiseWorld.trigger_event(wwise_world, "sfx_amb_warehouse")

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/options_menu.s2d/options_menu")
		--Register menu button mouse listener
		local custom_listener = OptionsMenu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, OptionsMenu.on_custom_event)
		OptionsMenu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local enter_game = function()
			OptionsMenu.action = "start"
			perform_action()
		end
		local exit = function()
			OptionsMenu.action = "exit"
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

	Appkit.manage_level_object(level, OptionsMenu, nil)
	setSlider()
end

function OptionsMenu.shutdown(object)
	if scaleform then
		scaleform.EventListener.disconnect(OptionsMenu.custom_listener)
		scaleform.Stingray.unload_project()
	end

	OptionsMenu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, OptionsMenu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
end

function OptionsMenu.on_custom_event(evt)
    if evt.name == "action" then
		if evt.data.message == "volume" then
			OptionsMenu.action = "volume"
			volume_f = evt.data.sliderValue
		elseif evt.data.message == "key bindings" then
			OptionsMenu.action = "key bindings"
		elseif evt.data.message == "graphics settings" then
		    OptionsMenu.action = "graphics settings"
		elseif evt.data.message == "back to main menu" then
		    OptionsMenu.action = "back to main menu"
		elseif evt.data.message == "mouse sensitivity" then
            OptionsMenu.action = "mouse sensitivity"
            mouseSens_f = evt.data.sliderValue
        end
    end
    if evt.name == "volume" then
        OptionsMenu.action = "volume"
        volume_f = evt.data.sliderValue
    elseif evt.name == "mouse sensitivity" then
        OptionsMenu.action = "mouse sensitivity"
        mouseSens_f = evt.data.sliderValue
    end
end

-- [[Main Menu custom functionality]]--
function OptionsMenu.update(object, dt)
	if OptionsMenu.debug_menu then
		OptionsMenu.debug_menu:update()
	end

	if OptionsMenu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					OptionsMenu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					OptionsMenu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				OptionsMenu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				OptionsMenu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

function setSlider()
    local widget = scaleform.Stage.actor_by_name_path("options_menu.main.VolumeSlider")
    local widgetActor = scaleform.Actor.component_by_name(widget, "WidgetBase")
    local volumeSlider = scaleform.ScriptComponent.script_results(widgetActor)
    volumeSlider.setValue(internalValues.settings.volume_f)
    
    local widget = scaleform.Stage.actor_by_name_path("options_menu.main.Mouse_Sensitivity_Slider")
    local widgetActor = scaleform.Actor.component_by_name(widget, "WidgetBase")
    local mouseSlider = scaleform.ScriptComponent.script_results(widgetActor)
    -- if(mouseSens_f ~= nil) then
    --     mouseSlider.setValue(mouseSens_f)
    -- else
    --     mouseSlider.setValue(internalValues.settings.mouseSens_f)
    -- end
    mouseSlider.setValue(internalValues.settings.mouseSens_f*1000)
end

return OptionsMenu
