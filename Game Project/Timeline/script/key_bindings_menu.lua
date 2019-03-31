local Util = require 'core/appkit/lua/util'
local SimpleProject = require 'core/appkit/lua/simple_project'
local DebugMenu = require 'core/appkit/lua/debug_menu'
local internalValues = require 'script/lua/agentInternalData'
require 'script/lua/input_mapper'
require 'script/lua/load_save'

Project.KeyBindingsMenu = Project.KeyBindingsMenu or {}
local KeyBindingsMenu = Project.KeyBindingsMenu

KeyBindingsMenu.custom_listener = KeyBindingsMenu.custom_listener or nil
KeyBindingsMenu.action = nil

local sound_W = stingray.Wwise.wwise_world(stingray.Application.main_world())
local start_time = 0
local volume_f
global_changedButton_b = false 
local selectedButton_s
local UI_timer_f = 0
local frameSkip_b = false

local WwiseWorld = stingray.WwiseWorld
local button_sound_w = nil

local function perform_action()
	if KeyBindingsMenu.action == "back to options" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
	    KeyBindingsMenu.shutdown()
	    OptionsMenu.start()
	elseif KeyBindingsMenu.action == "drop" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
	    frameSkip_b = true
	    global_changedButton_b = true
	    selectedButton_s = "drop"
	elseif KeyBindingsMenu.action == "throw" then
	    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
	    
	    frameSkip_b = true
	    global_changedButton_b = true
	    selectedButton_s = "throw"
    elseif KeyBindingsMenu.action == "run" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "run"
    elseif KeyBindingsMenu.action == "move left" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "move left"
    elseif KeyBindingsMenu.action == "move right" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "move right"
    elseif KeyBindingsMenu.action == "move forward" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "move forward"
    elseif KeyBindingsMenu.action == "move backwards" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "move backwards"
    elseif KeyBindingsMenu.action == "crouch" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "crouch"
    elseif KeyBindingsMenu.action == "interact" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "interact"
    elseif KeyBindingsMenu.action == "timeshift" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "timeshift"
    elseif KeyBindingsMenu.action == "jump" then
        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
        button_sound_w = WwiseWorld.trigger_event(wwise_world, "sfx_ui_punk_enter_01")
        
        frameSkip_b = true
        global_changedButton_b = true
        selectedButton_s = "jump"
    end
	KeyBindingsMenu.action = nil
	button_sound_w = nil
end

function KeyBindingsMenu.start()
	if stingray.Window then
		stingray.Window.set_show_cursor(true)
	end

	if scaleform then
		scaleform.Stingray.load_project_and_scene("content/ui/key_bindings_menu.s2d/key_bindings_menu")
		--Register menu button mouse listener
		local custom_listener = KeyBindingsMenu.custom_listener
		custom_listener = scaleform.EventListener.create(custom_listener, KeyBindingsMenu.on_custom_event)
		KeyBindingsMenu.custom_listener = custom_listener
		scaleform.EventListener.connect(custom_listener, scaleform.EventTypes.Custom)
	else
		local back_to_options = function()
			KeyBindingsMenu.action = "back to options"
			perform_action()
		end
		local jump = function()
			KeyBindingsMenu.action = "jump"
			perform_action()
		end
		local run = function()
			KeyBindingsMenu.action = "run"
			perform_action()
		end
		local crouch = function()
			KeyBindingsMenu.action = "crouch"
			perform_action()
		end
		local throw = function()
			KeyBindingsMenu.action = "throw"
			perform_action()
		end
		local drop = function()
			KeyBindingsMenu.action = "drop"
			perform_action()
		end
		local interact = function()
			KeyBindingsMenu.action = "interact"
			perform_action()
		end
		local move_left = function()
			KeyBindingsMenu.action = "move left"
			perform_action()
		end
		local move_right = function()
			KeyBindingsMenu.action = "move right"
			perform_action()
		end
		local move_forward = function()
			KeyBindingsMenu.action = "move forward"
			perform_action()
		end
		local move_backwards = function()
			KeyBindingsMenu.action = "move backwards"
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

	Appkit.manage_level_object(level, KeyBindingsMenu, nil)
    setUIForButtons()
end

function KeyBindingsMenu.shutdown(object)
	if scaleform then
		scaleform.EventListener.disconnect(KeyBindingsMenu.custom_listener)
		scaleform.Stingray.unload_project()
	end

	KeyBindingsMenu.evt_listener_handle = nil
	Appkit.unmanage_level_object(SimpleProject.level, KeyBindingsMenu, nil)
	if stingray.Window then
		stingray.Window.set_show_cursor(false)
	end
end

function KeyBindingsMenu.on_custom_event(evt)
    if evt.name == "action" then
		if evt.data.message == "back to options" then
			KeyBindingsMenu.action = "back to options"
		elseif evt.data.message == "drop" then
		    KeyBindingsMenu.action = "drop"
		elseif evt.data.message == "throw" then
		    KeyBindingsMenu.action = "throw"
	    elseif evt.data.message == "run" then
	        KeyBindingsMenu.action = "run"
	    elseif evt.data.message == "move left" then
	        KeyBindingsMenu.action = "move left"
	    elseif evt.data.message == "move right" then
	        KeyBindingsMenu.action = "move right"
	    elseif evt.data.message == "move forward" then
	        KeyBindingsMenu.action = "move forward"
	    elseif evt.data.message == "move backwards" then
	        KeyBindingsMenu.action = "move backwards"
	    elseif evt.data.message == "crouch" then
	        KeyBindingsMenu.action = "crouch"
	    elseif evt.data.message == "interact" then
	        KeyBindingsMenu.action = "interact"
	    elseif evt.data.message == "timeshift" then
	        KeyBindingsMenu.action = "timeshift"
	   elseif evt.data.message == "jump" then
	        KeyBindingsMenu.action = "jump"
	    end
    end
end

-- [[Main Menu custom functionality]]--
function KeyBindingsMenu.update(object, dt)
    BindKeys()
	if KeyBindingsMenu.debug_menu then
		KeyBindingsMenu.debug_menu:update()
	end

	if KeyBindingsMenu.action == nil  then
		local time = stingray.World.time(SimpleProject.world)
		local p = stingray.Application.platform()
		if time - start_time > 1 then
			if Appkit.Util.is_pc() then
				if stingray.Keyboard.pressed(stingray.Keyboard.button_id("1")) then
					KeyBindingsMenu.action = "start"
				elseif stingray.Keyboard.pressed(stingray.Keyboard.button_id("esc")) then
					KeyBindingsMenu.action = "exit"
				end
			elseif p == stingray.Application.XB1 or p == stingray.Application.PS4 then 
				if stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "a", nil, "cross"))) then
    				KeyBindingsMenu.action = "start"
    			elseif stingray.Pad1.pressed(stingray.Pad1.button_id(Appkit.Util.plat(nil, "b", nil, "circle"))) then
    				KeyBindingsMenu.action = "exit"
    			end
    		end

		end
	end
	perform_action()
end

--Function that binds the keys to selected actions by player
function BindKeys()
    --Timer for the UI-text which appears if one tries to bind a key that already is bound to another action
    UI_timer_f = UI_timer_f + 1
    if(UI_timer_f >=150)then
            change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "close warning text",
                }
            scaleform.Stage.dispatch_event(change_button_UI_event)
        end
        if(frameSkip_b) then
            frameSkip_b = false
            return
        end
    --first we have to check if any button has been pressed to be changed
    if(global_changedButton_b) then
        local keyPressed_i = stingray.Keyboard.any_pressed()
        local mousePressed_i = stingray.Mouse.any_pressed()
        local change_button_UI_event = nil
        
        --then we have to check if the player has given an input to change the key binding
        if(keyPressed_i ~= nil or mousePressed_i ~= nil) then
            for key, value in pairs(internalValues.settings) do
                if(value == keyPressed_i) then
                    
                    UI_timer_f = 0
                    change_button_UI_event = 
                    {
                        eventId = scaleform.EventTypes.Custom,
                        name = "wrong binding"
                    }
                    scaleform.Stage.dispatch_event(change_button_UI_event)
                    global_changedButton_b = false
                    return
                end
                -- if(value == mousePressed_i) then
                --     UI_timer_f = 0
                    
                --     change_button_UI_event = 
                --     {
                --         eventId = scaleform.EventTypes.Custom,
                --         name = "wrong binding"
                --     }
                --     scaleform.Stage.dispatch_event(change_button_UI_event)
                --     global_changedButton_b = false
                --     return
                -- end
            end
        end
        
        if(keyPressed_i ~= nil) then
            global_changedButton_b = false
            if(selectedButton_s == "run") then
                internalValues.settings.run_f = keyPressed_i
                internalValues.playerSettings.run_km = stingray.Keyboard
                internalValues.saveDataStrings.run_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change run",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "crouch" then
                internalValues.settings.crouch_f = keyPressed_i
                internalValues.playerSettings.crouch_km = stingray.Keyboard
                internalValues.saveDataStrings.crouch_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change crouch",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "jump" then
                internalValues.settings.jump_f = keyPressed_i
                internalValues.playerSettings.jump_km = stingray.Keyboard
                internalValues.saveDataStrings.jump_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change jump",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "timeshift" then
                internalValues.settings.timeshift_f = keyPressed_i
                internalValues.playerSettings.timeshift_km = stingray.Keyboard
                internalValues.saveDataStrings.timeshift_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change timeshift",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "drop" then
                internalValues.settings.drop_f = keyPressed_i
                internalValues.playerSettings.drop_km = stingray.Keyboard
                internalValues.saveDataStrings.drop_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change drop",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "throw" then
                internalValues.settings.throw_f = keyPressed_i
                internalValues.playerSettings.throw_km = stingray.Keyboard
                internalValues.saveDataStrings.throw_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change throw",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "move right" then
                internalValues.settings.right_f = keyPressed_i
                internalValues.playerSettings.right_km = stingray.Keyboard
                internalValues.saveDataStrings.right_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change right",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "move left" then
                internalValues.settings.left_f = keyPressed_i
                internalValues.playerSettings.left_km = stingray.Keyboard
                internalValues.saveDataStrings.left_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change left",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "move forward" then
                internalValues.settings.forward_f = keyPressed_i
                internalValues.playerSettings.forward_km = stingray.Keyboard
                internalValues.saveDataStrings.forward_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change forward",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "move backwards" then
                internalValues.settings.backwards_f = keyPressed_i
                internalValues.playerSettings.backwards_km = stingray.Keyboard
                internalValues.saveDataStrings.backwards_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change backwards",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            elseif selectedButton_s == "interact" then
                internalValues.settings.interact_f = keyPressed_i
                internalValues.playerSettings.interact_km = stingray.Keyboard
                internalValues.saveDataStrings.interact_km = "Keyboard"
                
                change_button_UI_event = 
                {
                    eventId = scaleform.EventTypes.Custom,
                    name = "change interact",
                    data = stingray.Keyboard.button_name(keyPressed_i)
                }
                
            end
            scaleform.Stage.dispatch_event(change_button_UI_event)
        end
    --     if(mousePressed_i ~= nil) then
    --         global_changedButton_b = false
    --         if(selectedButton_s == "run") then
    --             internalValues.settings.run_f = mousePressed_i
    --             internalValues.playerSettings.run_km = stingray.Mouse
    --             internalValues.saveDataStrings.run_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change run",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "crouch" then
    --             internalValues.settings.crouch_f = mousePressed_i
    --             internalValues.playerSettings.crouch_km = stingray.Mouse
    --             internalValues.saveDataStrings.crouch_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change crouch",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "jump" then
    --             internalValues.settings.jump_f = mousePressed_i
    --             internalValues.playerSettings.jump_km = stingray.Mouse
    --             internalValues.saveDataStrings.jump_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change jump",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "timeshift" then
    --             internalValues.settings.timeshift_f = mousePressed_i
    --             internalValues.playerSettings.timeshift_km = stingray.Mouse
    --             internalValues.saveDataStrings.timeshift_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change timeshift",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "drop" then
    --             internalValues.settings.drop_f = mousePressed_i
    --             internalValues.playerSettings.drop_km = stingray.Mouse
    --             internalValues.saveDataStrings.drop_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change drop",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "throw" then
    --             internalValues.settings.throw_f = mousePressed_i
    --             internalValues.playerSettings.throw_km = stingray.Mouse
    --             internalValues.saveDataStrings.throw_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change throw",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "move right" then
    --             internalValues.settings.right_f = mousePressed_i
    --             internalValues.playerSettings.right_km = stingray.Mouse
    --             internalValues.saveDataStrings.right_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change right",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "move left" then
    --             internalValues.settings.left_f = mousePressed_i
    --             internalValues.playerSettings.left_km = stingray.Mouse
    --             internalValues.saveDataStrings.left_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change left",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "move forward" then
    --             internalValues.settings.forward_f = mousePressed_i
    --             internalValues.playerSettings.forward_km = stingray.Mouse
    --             internalValues.saveDataStrings.forward_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change forward",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "move backwards" then
    --             internalValues.settings.backwards_f = mousePressed_i
    --             internalValues.playerSettings.backwards_km = stingray.Mouse
    --             internalValues.saveDataStrings.backwards_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change backwards",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
                
    --         elseif selectedButton_s == "interact" then
    --             internalValues.settings.interact_f = mousePressed_i
    --             internalValues.playerSettings.interact_km = stingray.Mouse
    --             internalValues.saveDataStrings.interact_km = "Mouse"
    --             change_button_UI_event = 
    --             {
    --                 eventId = scaleform.EventTypes.Custom,
    --                 name = "change interact",
    --                 data = stingray.Mouse.button_name(mousePressed_i)
    --             }
    --         end
    --         scaleform.Stage.dispatch_event(change_button_UI_event)
    --     end
    end
end
--end of BindKeys()

--Function which sets the UI for the binded keys. This is called when a new binding has been made
function setUIForButtons()
    local change_button_UI_event = nil
    change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change crouch",
            data = internalValues.playerSettings.crouch_km.button_name(internalValues.settings.crouch_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change run",
            data = internalValues.playerSettings.run_km.button_name(internalValues.settings.run_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change interact",
            data = internalValues.playerSettings.interact_km.button_name(internalValues.settings.interact_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change jump",
            data = internalValues.playerSettings.jump_km.button_name(internalValues.settings.jump_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change left",
            data = internalValues.playerSettings.left_km.button_name(internalValues.settings.left_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change right",
            data = internalValues.playerSettings.right_km.button_name(internalValues.settings.right_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change backwards",
            data = internalValues.playerSettings.backwards_km.button_name(internalValues.settings.backwards_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change forward",
            data = internalValues.playerSettings.forward_km.button_name(internalValues.settings.forward_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change timeshift",
            data = internalValues.playerSettings.timeshift_km.button_name(internalValues.settings.timeshift_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change throw",
            data = internalValues.playerSettings.throw_km.button_name(internalValues.settings.throw_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
        change_button_UI_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "change drop",
            data = internalValues.playerSettings.drop_km.button_name(internalValues.settings.drop_f)
        }
        scaleform.Stage.dispatch_event(change_button_UI_event)
end-- end of setUIForButtons()

return KeyBindingsMenu
