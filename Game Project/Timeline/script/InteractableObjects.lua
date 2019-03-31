require 'core/appkit/lua/class'
require 'core/appkit/lua/app'
require 'script/lua/Inventory'

local InternalValues = InternalValues or require "script/lua/agentInternalData"

InteractableObjects = Appkit.class(InteractableObjects)
package.loaded[...] = InteractableObjects

local Wwise = stingray.Wwise
local WwiseWorld = stingray.WwiseWorld
local wwise_world = nil

local world_w = stingray.Application.main_world()
local scream_b = false
local lastFootstepSound_f = 0
local keyAudio_w = nil
local lockerAudio_w = nil
local batteryAudio_w = nil
local doorAudio_w = nil
local lockedLockerAudio_w = nil
local lockedDoorAudio_w = nil							 

local currentLoadedRoom_l = nil
local roomOne = nil
local roomTwo = nil
local roomThree = nil

--Initilizes the class 
function InteractableObjects:init(type_s)
end--End of init()

--Checks the type of the object which the crosshair is aiming on
--@Param unit_u, the unit the player is looking at
--@Param isLookedAt_b, will always be true when this function is called
function InteractableObjects:CheckUnitType(unit_u, isLookedAt_b)
    if (stingray.Unit.alive(unit_u)) then
        local unitType_s = stingray.Unit.get_data(unit_u, "Type")
        local interact_hud_event = nil
        if(unitType_s == "Key") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
                global_inventoryManager_o:InsertToSelected(unitType_s, unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_ooh")
                end
            end
        elseif(unitType_s == "default") then
            return
        elseif(unitType_s == "Rock") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
                global_inventoryManager_o:InsertToSelected(unitType_s, unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_ooh")
                end
            end
        elseif(unitType_s == "Seeds") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
                global_inventoryManager_o:InsertToSelected(unitType_s, unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_ooh")
                end
            end
        elseif(unitType_s == "Critter") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
                global_inventoryManager_o:InsertToSelected(unitType_s, unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_critter_interaction")
                end
            end
        elseif(unitType_s == "Lever") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:ButtonLeverActive(unit_u)
            end
        elseif(unitType_s == "Box") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:PushObject(unit_u)
                --InteractableObjects:PushPillar(unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_jump_ugh")
                end
            end
        elseif(unitType_s == "Cogwheel") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
                global_inventoryManager_o:InsertToSelected(unitType_s, unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_ooh")
                end
            end
        elseif(unitType_s == "Battery") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
                global_inventoryManager_o:InsertToSelected(unitType_s, unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_ooh")
                end
            end
        elseif(unitType_s == "Door") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                if(stingray.Unit.get_data(unit_u, "Locked")) then
                    InteractableObjects:Unlock(unit_u)
                elseif(not stingray.Unit.get_data(unit_u, "Connected")) then
                    InteractableObjects:Open(unit_u)
                end
            end
        elseif(unitType_s == "DirtPile") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:PlantSeed(unit_u)
            end
        elseif(unitType_s == "CogwheelSlot") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:PlaceCogwheel(unit_u)
            end
        elseif(unitType_s == "Engine") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:PlaceBattery(unit_u)
            end
        elseif(unitType_s == "Locker") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                if(stingray.Unit.get_data(unit_u, "Locked")) then
                    InteractableObjects:Unlock(unit_u)
                else
                    InteractableObjects:Open(unit_u)
                end
            end
        elseif(unitType_s == "Lever Slot") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:PlaceLever(unit_u, isPlaced_b)
            end
        elseif(unitType_s == "Loose Lever") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
                global_inventoryManager_o:InsertToSelected(unitType_s, unit_u)
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    WwiseWorld.trigger_event(wwise_world, "vo_ooh")
                end
            end
        elseif(unitType_s == "Time Anchor") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                if(stingray.Unit.get_data(unit_u, "Start/End") == "End") then
                    global_changeAndSave_b = true
                end
            end
        elseif(unitType_s == "PowerPanel") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:PlacePower(unit_u)
            end
         elseif(unitType_s == "Two Slot Panel") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:TwoPanelPlace(unit_u)
            end
        elseif(unitType_s == "Combination Wheel") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:CombinationWheel(unit_u)
            end
        elseif(unitType_s == "PowerPillar") then
            interact_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "interaction",
                data = {interactionType = unitType_s, visible = isLookedAt_b}
            }
            if(Appkit.input_mapper:Interact()) then
                InteractableObjects:PushObject(unit_u)
            end
        elseif(unitType_s == "Target") then
            if(Appkit.input_mapper:Interact()) then
               SimpleProject.change_level(Project.level_names.test)
            end    
        end
        if (interact_hud_event == nil) then return end
        scaleform.Stage.dispatch_event(interact_hud_event)
    end
end--End of CheckUnitType()

--Method that adds and impulse to the unit
--@Param unit_u, the unit we want to push
function InteractableObjects:PushObject(unit_u)
    local unitType_s = stingray.Unit.get_data(unit_u, "Type")
    local inDungeon_b = stingray.Unit.get_data(unit_u, "inDungeon_b")
    if(unitType_s == "PowerPillar") then
        if(inDungeon_b) then
            local movablePillars_t = stingray.World.units_by_resource(stingray.Application.main_world(), "content/_Assets/Key Items/Power_Pillar/Geo_PowerPillar_Placeholder")
            local pillarID_i = stingray.Unit.get_data(unit_u, "pairID_i")
            for k, pillar_u in pairs(movablePillars_t) do
                local twin_i = stingray.Unit.get_data(pillar_u, "pairID_i")
                local inRuins_b = stingray.Unit.get_data(pillar_u, "inDungeon_b")
                if(twin_i == pillarID_i and not inRuins_b) then
                    local rotation_q = stingray.Unit.world_rotation(global_player_u, 1)
                    local forward_vec3 = stingray.Quaternion.forward(rotation_q)
                    local actor = stingray.Unit.actor(unit_u,1)
                    stingray.Actor.set_velocity(actor,stingray.Vector3.multiply(forward_vec3,3))
                    InteractableObjects:ResetRuinObject(unit_u, pillar_u) --To reset the position of the ruin box
                    
                    local twinActor_a = stingray.Unit.actor(pillar_u,1)
                    stingray.Actor.set_velocity(twinActor_a,stingray.Vector3.multiply(forward_vec3,3))

                    break
                end
            end
        else
            local rotation_q = stingray.Unit.world_rotation(global_player_u,1)
            local forward_vec3 = stingray.Quaternion.forward(rotation_q)
            local actor = stingray.Unit.actor(unit_u,1)
            stingray.Actor.set_velocity(actor,stingray.Vector3.multiply(forward_vec3,3))
            stingray.Actor.add_impulse(actor,forward_vec3)
            if Wwise then
    		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
    		    WwiseWorld.trigger_event(wwise_world, "sfx_sliding_boxes")
    	    end
        end
    elseif(unitType_s == "Box") then
        if(inDungeon_b) then
            local movableBoxes_t = stingray.World.units_by_resource(stingray.Application.main_world(), "content/_Assets/Key Items/Box/Geo_Box_Placeholder")
            local boxID_i = stingray.Unit.get_data(unit_u, "pairID_i")
            for k, box_u in pairs(movableBoxes_t) do
                local twin_i = stingray.Unit.get_data(box_u, "pairID_i")
                local inRuins_b = stingray.Unit.get_data(box_u, "inDungeon_b")
                if(twin_i == boxID_i and not inRuins_b) then
                    InteractableObjects:ResetRuinObject(unit_u, box_u) --To reset the position of the ruin box
                    local rotation_q = stingray.Unit.world_rotation(global_player_u, 1)
                    local forward_vec3 = stingray.Quaternion.forward(rotation_q)
                    local actor = stingray.Unit.actor(unit_u,1)
                    local twinActor_a = stingray.Unit.actor(box_u,1)
                    stingray.Actor.set_velocity(twinActor_a,stingray.Vector3.multiply(forward_vec3,3))
                    break
                end
            end
        else
            local rotation_q = stingray.Unit.world_rotation(global_player_u,1)
            local forward_vec3 = stingray.Quaternion.forward(rotation_q)
            local actor = stingray.Unit.actor(unit_u,1)
            stingray.Actor.set_velocity(actor,stingray.Vector3.multiply(forward_vec3,3))
            stingray.Actor.add_impulse(actor,forward_vec3)
            if Wwise then
    		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
    		    WwiseWorld.trigger_event(wwise_world, "sfx_sliding_boxes")
    	    end
        end
    end
end--End of PushObject()

--Method that resets the box in the ruins timeline
--@Param dungeonUnit_u, the box in the dungeon
--@Param ruinUnit_u, the box in the ruins
function InteractableObjects:ResetRuinObject(dungeonUnit_u, ruinUnit_u)
    local timeshiftSafety_u = stingray.World.unit_by_name(stingray.Application.main_world(), "Dunkel_Crouch_Unit")
    local xOffset_f = stingray.Unit.get_data(timeshiftSafety_u, "xOffset_f")
    local yOffset_f = stingray.Unit.get_data(timeshiftSafety_u, "yOffset_f")
    local zOffset_f = stingray.Unit.get_data(timeshiftSafety_u, "zOffset_f")
    
    local offset_vec3 = stingray.Vector3(xOffset_f, yOffset_f, zOffset_f)
    local dungeonPosition_vec3 = stingray.Unit.local_position(dungeonUnit_u,1)
    local dungeonActor_a = stingray.Unit.actor(dungeonUnit_u,1)
    local dungeonRotation_q = stingray.Actor.rotation(dungeonActor_a)
    
    local ruinsActor_a = stingray.Unit.actor(ruinUnit_u, 1)
    stingray.Actor.teleport_position(ruinsActor_a, dungeonPosition_vec3 + offset_vec3)
    stingray.Actor.teleport_rotation(ruinsActor_a, dungeonRotation_q)
end

--When a button is pressed or lever is pulled this function will trigger the right reaction (ex. open door etc.)
--@Param unit_u, the unit that is being aimed on and to be interacted with
function InteractableObjects:ButtonLeverActive(unit_u)
    
    local affectedUnit_s = stingray.Unit.get_data(unit_u, "Affected Unit")
    local affectedUnit_u = stingray.World.unit_by_name(stingray.Application.main_world(), affectedUnit_s)
    if(affectedUnit_s == "default" or affectedUnit_u == nil) then
        return
    end
    local affectedUnitType_s = stingray.Unit.get_data(affectedUnit_u, "Type")
    local isActivated_b = stingray.Unit.get_data(unit_u, "isPushed_b")
    local isPlaced_b = stingray.Unit.get_data(unit_u, "Placed")
    local isPullable_b = stingray.Unit.get_data(unit_u, "Pullable")
    local pairID_i = stingray.Unit.get_data(unit_u, "pairID_i")

    if(isPlaced_b and isPullable_b) then
        if(affectedUnitType_s == "Door") then
            local doorLocked_b = stingray.Unit.get_data(affectedUnit_u, "Locked")
            if(not doorLocked_b) then
                if(not isActivated_b) then
                    if(pairID_i == 1) then
                        stingray.Unit.set_data(affectedUnit_u, "inputOne_b", true)
                    elseif(pairID_i == 2) then
                        stingray.Unit.set_data(affectedUnit_u, "inputTwo_b", true)
                    else
                        InteractableObjects:Open(affectedUnit_u)
                    end
                    stingray.Unit.set_data(unit_u, "isPushed_b", true)
                    stingray.Unit.animation_event(unit_u, "PullDown")
                else
                    if(pairID_i == 1) then
                        stingray.Unit.set_data(affectedUnit_u, "inputOne_b", false)
                    elseif(pairID_i == 2) then
                        stingray.Unit.set_data(affectedUnit_u, "inputTwo_b", false)
                    else
                        InteractableObjects:Open(affectedUnit_u)
                    end
                    stingray.Unit.set_data(unit_u, "isPushed_b", false)
                    stingray.Unit.animation_event(unit_u, "PullUp")
                end
            end
        elseif(affectedUnitType_s == "CogwheelSlot") then
            local cWheelPlaced_b = stingray.Unit.get_data(affectedUnit_u, "Placed")
            if(cWheelPlaced_b) then
                local temp_s = stingray.Unit.get_data(affectedUnit_u, "Affected Unit")
                local temp_u = stingray.World.unit_by_name(stingray.Application.main_world(), temp_s)
                local doorLocked_b = stingray.Unit.get_data(temp_u, "Locked")
                
                if(not doorLocked_b) then
                    if(not isActivated_b) then
                        InteractableObjects:Open(affectedUnit_u)
                        stingray.Unit.set_data(unit_u, "isPushed_b", true)
                        stingray.Unit.animation_event(unit_u, "PullDown")
                    else
                        InteractableObjects:Open(affectedUnit_u)
                        stingray.Unit.set_data(unit_u, "isPushed_b", false)
                        stingray.Unit.animation_event(unit_u, "PullUp")
                    end
                end
            end
        elseif(affectedUnitType_s == "Lever") then
            if(not isActivated_b) then
                stingray.Unit.set_data(affectedUnit_u, "Pullable", true)
                stingray.Unit.animation_event(unit_u, "PullDown")
            else
                stingray.Unit.set_data(affectedUnit_u, "Pullable", false)
                stingray.Unit.animation_event(unit_u, "PullUp")
            end
        elseif(affectedUnitType_s == "loader") then
            if(not isActivated_b) then
                stingray.Unit.set_data(unit_u, "isPushed_b", true)
                stingray.Unit.animation_event(unit_u, "PullDown")
                local wp = stingray.World.unit_by_name(stingray.Application.main_world(), "room_wp")
                local pos = stingray.Unit.world_position(wp, 1)
                currentLoadedRoom_l = stingray.World.load_level(stingray.Application.main_world(), "content/_Assets/Levels/Level_4_Room_1",pos )
            else
                stingray.Unit.set_data(unit_u, "isPushed_b", false)
                stingray.World.destroy_level(stingray.Application.main_world(),currentLoadedRoom_l )
                stingray.Unit.animation_event(unit_u, "PullUp")
            end
        elseif(affectedUnitType_s == "PrisonDoor") then
            if(not isActivated_b) then
                stingray.Unit.animation_event(affectedUnit_u, "Clopen")
                stingray.Unit.set_data(unit_u, "Pullable", true)
                stingray.Unit.set_data(unit_u, "Type", "done")
                stingray.Unit.animation_event(unit_u, "PullDown")
            end
        elseif(affectedUnitType_s == "EndGame") then
            global_EndGame = true
            global_winMusic_b = true
            stingray.Unit.animation_event(unit_u, "PullDown")
        end
    else
        InteractableObjects:PlaceLever(unit_u, isPlaced_b)
    end
end--End of ButtonPressed()

--If the player aims at a door and is holding a key, the aimed at door will be unlocked.
--@Param unit_u, the unit that is being aimed on and to be interacted with
function InteractableObjects:Unlock(unit_u)
    local isLocked_b = stingray.Unit.get_data(unit_u, "Locked")
    local type_s = stingray.Unit.get_data(unit_u, "Type")
    
    --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
    if(isLocked_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Key") then
        stingray.Unit.set_data(unit_u, "Locked", false)
        global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
        
        empty_selected_hud_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "empty_selected",
            data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
        }
        scaleform.Stage.dispatch_event(empty_selected_hud_event)
        
        if(type_s == "Locker") then
        end
    else
        if(type_s == "Locker") then
            stingray.Unit.animation_event(unit_u, "Locked")
            
            if(keyAudio_w == nil) then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    keyAudio_w = WwiseWorld.trigger_event(wwise_world, "vo_need_key")
                end
            elseif(keyAudio_w ~= nil) then
                if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()),keyAudio_w)) then
                    if Wwise then
                        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                        keyAudio_w = WwiseWorld.trigger_event(wwise_world, "vo_need_key")
                    end
                end
            end
        
            if(lockedLockerAudio_w == nil) then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    lockedLockerAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_locked_locker")
                end
            elseif(lockedLockerAudio_w ~= nil) then
                if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()),lockedLockerAudio_w)) then
                    if Wwise then
                        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                        lockedLockerAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_locked_locker")
                    end
                end
            end
        
        elseif(stingray.Unit.get_data(unit_u, "Type") == "Door") then
            if(keyAudio_w == nil) then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    keyAudio_w = WwiseWorld.trigger_event(wwise_world, "vo_need_key")
                end
            elseif(keyAudio_w ~= nil) then
                if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()),keyAudio_w)) then
                    if Wwise then
                        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                        keyAudio_w = WwiseWorld.trigger_event(wwise_world, "vo_need_key")
                    end
                end
            end
        
            if(lockedDoorAudio_w == nil) then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    lockedDoorAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_locked")
                end
            elseif(lockedDoorAudio_w ~= nil) then
                if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()),lockedDoorAudio_w)) then
                    if Wwise then
                        wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                        lockedDoorAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_locked")
                    end
                end
            end		   
        end
    end
end--End of UnlockDoor()

function InteractableObjects:Open(unit_u)
    local open_b = stingray.Unit.get_data(unit_u, "isOpen_b")
    if(stingray.Unit.get_data(unit_u, "Type") == "Door") then
        local actor_a = stingray.Unit.actor(unit_u,"Geo_Door_Placeholder2")
        stingray.Actor.set_collision_filter(actor_a, "non_collider")
    elseif(stingray.Unit.get_data(unit_u, "Type") == "Locker") then
        local actorOne_a = stingray.Unit.actor(unit_u,"L_Door")
        local actorTwo_a = stingray.Unit.actor(unit_u,"R_Door")
        stingray.Actor.set_collision_filter(actorOne_a, "non_collider")
        stingray.Actor.set_collision_filter(actorTwo_a, "non_collider")
    end
    if(not open_b) then
        stingray.Unit.animation_event(unit_u, "Open")
        stingray.Unit.set_data(unit_u, "isOpen_b", true)
    else
        stingray.Unit.animation_event(unit_u, "Close")
        stingray.Unit.set_data(unit_u, "isOpen_b", false)
    end
    --play open/close door
    if(stingray.Unit.get_data(unit_u, "Type") == "Door") then
        
        if(doorAudio_w == nil) then
            if Wwise then
                wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                doorAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_open_door")
            end
        elseif(doorAudio_w ~= nil) then
            if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()),doorAudio_w)) then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    doorAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_open_door")
                end
            end
        end
        
    --play open/close locker 
    elseif(stingray.Unit.get_data(unit_u, "Type") == "Locker") then
        
        if(lockerAudio_w == nil) then
            if Wwise then
                wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                lockerAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_open_locker")
            end
        elseif(lockerAudio_w ~= nil) then
            if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()),lockerAudio_w)) then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    lockerAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_open_locker")
                end
            end
        end
    end   
end

--If the player aims at a dirt pile and is holding a seed, a seed will be planted in the dirt pile.
--@Param unit_u, the unit that is being aimed on and to be interacted with
function InteractableObjects:PlantSeed(unit_u)
    
    local isPlanted_b = stingray.Unit.get_data(unit_u, "Planted")
    local treeModel_s = stingray.Unit.get_data(unit_u, "treeToSpawn_s")
    --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
    if(not isPlanted_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Seeds") then
        local treePos_vec3 = stingray.Vector3(stingray.Unit.get_data(unit_u, "X"), stingray.Unit.get_data(unit_u, "Y"), stingray.Unit.get_data(unit_u, "Z"))
        local temp = stingray.World.spawn_unit(stingray.Application.main_world(), treeModel_s, treePos_vec3)
        table.insert(global_dynamicEntities_t, temp)
        stingray.Unit.set_data(unit_u, "Planted", true)
        global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
        empty_selected_hud_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "empty_selected",
            data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
        }
        scaleform.Stage.dispatch_event(empty_selected_hud_event)
        
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_plant_seed")
	    end
        
    end
end--End of PlantSeed()

--If the player aims at a cogwheel slot and is holding a cogwheel, a cogwheel will be placed in the slot.
--@Param unit_u, the unit that is being aimed on and to be interacted with
function InteractableObjects:PlaceCogwheel(unit_u)
    
    local isPlaced_b = stingray.Unit.get_data(unit_u, "Placed")
    local spawnPos_vec3 = stingray.Vector3.add(stingray.Unit.world_position(unit_u, 1), stingray.Vector3(-0.1, -0.1, 0))
    local cWheelRot_q = stingray.Unit.world_rotation(unit_u, 1)
    
    --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
    if(not isPlaced_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Cogwheel") then
        
        local cWheelNum_i = stingray.Unit.get_data(unit_u, "cWheelNum_i")
        local cWheel_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Cogs/Geo_Cog_"..cWheelNum_i.."_Placeholder", spawnPos_vec3)
        table.insert(global_dynamicEntities_t, cWheel_u)
        stingray.Unit.set_local_rotation(cWheel_u, 1, cWheelRot_q)
        stingray.Unit.disable_physics(cWheel_u)
        stingray.Unit.set_data(unit_u, "Placed", true)
        
        global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
        empty_selected_hud_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "empty_selected",
            data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
        }
        scaleform.Stage.dispatch_event(empty_selected_hud_event)
    end
end--End of PlaceCogwheel()

--If the player aims at a generator and is holding a battery, a battery will be placed in the generator.
--@Param unit_u, the unit that is being aimed on and to be interacted with
function InteractableObjects:PlaceBattery(unit_u)
    local isPlaced_b = stingray.Unit.get_data(unit_u, "Placed")
    
    local affectedUnit_s = stingray.Unit.get_data(unit_u, "Affected Unit")
    local affectedUnitRuins_s = stingray.Unit.get_data(unit_u, "Affected Unit T2")
    local lerpAffected_b = stingray.Unit.get_data(unit_u, "lerpAffected_b")
    local affectedUnit_u = stingray.World.unit_by_name(stingray.Application.main_world(), affectedUnit_s)
    local affectedUnitRuins_u = stingray.World.unit_by_name(stingray.Application.main_world(), affectedUnitRuins_s)
    local affectedType_s = stingray.Unit.get_data(affectedUnit_u, "Type")
    if(affectedUnit_s == "default" or affectedUnit_u == nil) then
        return
    end
    --local affectedUnitType_s = stingray.Unit.get_data(affectedUnit_u, "Type")
    local position_node = stingray.Unit.node(unit_u, "position")
    local genScale_vec3 = stingray.Unit.local_scale(unit_u, 1)
    local genRot_q = stingray.Unit.local_rotation(unit_u, 1)
    local spawnPos_vec3 = stingray.Unit.world_position(unit_u, position_node) + stingray.Vector3(0, 0, 0.075)
    
    if(not isPlaced_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] ~= "Battery") then
        
        if(batteryAudio_w == nil) then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    batteryAudio_w = WwiseWorld.trigger_event(wwise_world, "vo_need_battery")
                elseif(batteryAudio_w ~= nil) then
                    if(not stingray.WwiseWorld.is_playing(stingray.Wwise.wwise_world(stingray.Application.main_world()),batteryAudio_w)) then
                        if Wwise then
                            wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                            batteryAudio_w = WwiseWorld.trigger_event(wwise_world, "vo_need_battery")
                        end
                    end
                end			   
        end
    end
    
    --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
    if(not isPlaced_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Battery") then
        
        local pwrSrc_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", spawnPos_vec3)
        table.insert(global_dynamicEntities_t, pwrSrc_u)
        stingray.Unit.disable_physics(pwrSrc_u)
        stingray.Unit.set_local_rotation(pwrSrc_u, 1, genRot_q)
        stingray.Unit.set_local_scale(pwrSrc_u, 1, genScale_vec3)
        
        stingray.Unit.set_data(unit_u, "pwrSrc_u", pwrSrc_u)
        stingray.Unit.set_data(unit_u, "Placed", true)
        global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
        stingray.Unit.animation_event(unit_u, "Activate")
        
        if (affectedUnit_s == "TimeDisruptor") then
            stingray.Unit.set_data(affectedUnit_u, "Generators",(stingray.Unit.get_data(affectedUnit_u, "Generators") + 1) )
        elseif(affectedType_s == "Door") then
            stingray.Unit.set_data(affectedUnit_u, "inputOne_b",true)
            stingray.Unit.set_data(affectedUnit_u, "inputTwo_b",true)
            stingray.Unit.set_data(affectedUnit_u, "inputThree_b",true)
        else
            if(lerpAffected_b) then
                stingray.Unit.set_data(affectedUnit_u, "affected_b", true)
                if(affectedUnitRuins_u) then
                    stingray.Unit.set_data(affectedUnitRuins_u, "affected_b", true)
                end
            else
                stingray.Unit.set_local_position(affectedUnit_u, 1, stingray.Unit.world_position(affectedUnit_u, 1) + stingray.Vector3(3,0,0))
            end
        end
        empty_selected_hud_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "empty_selected",
            data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
        }
        scaleform.Stage.dispatch_event(empty_selected_hud_event)

    end
    
    if(isPlaced_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == nil) then
        local pwrSrc_u = stingray.Unit.get_data(unit_u, "pwrSrc_u")
        global_inventoryManager_o:InsertToSelected("Battery", pwrSrc_u)
        stingray.Unit.set_data(unit_u, "pwrSrc_u", nil)
        stingray.Unit.set_data(unit_u, "Placed", false)
        
        stingray.Unit.animation_event(unit_u, "Deactivate")
        if (affectedUnit_s == "TimeDisruptor") then
            stingray.Unit.set_data(affectedUnit_u, "Generators",(stingray.Unit.get_data(affectedUnit_u, "Generators") - 1) )
        else
             if(lerpAffected_b) then
                stingray.Unit.set_data(affectedUnit_u, "affected_b", false)
                stingray.Unit.set_data(affectedUnit_u, "reset_b", true)
                if(affectedUnitRuins_u) then
                    stingray.Unit.set_data(affectedUnitRuins_u, "affected_b", false)
                    stingray.Unit.set_data(affectedUnitRuins_u, "reset_b", true)
                end
            else
                stingray.Unit.set_local_position(affectedUnit_u, 1, stingray.Unit.world_position(affectedUnit_u, 1) + stingray.Vector3(-3,0,0))
            end
        end
    end
end--End of PlaceBattery()

--If the player aims at a lever slot and is holding a lever, a lever will be placed in the lever slot.
--@Param unit_u, the unit that is being aimed on and to be interacted with
function InteractableObjects:PlaceLever(unit_u, isPlaced_b)

    --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
    if(not isPlaced_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Loose Lever") then
        local spawnPos_vec3 = stingray.Unit.world_position(unit_u, 1)
        local spawnRot_q = stingray.Unit.world_rotation(unit_u, 1)
        local affectedUnit_s = stingray.Unit.get_data(unit_u, "Affected Unit")
        local pairID_i = stingray.Unit.get_data(unit_u, "pairID_i")
        
        stingray.Unit.set_data(unit_u, "despawn_b", true)
        global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
        
        unit_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Levers/Geo_KeyItem_Lever", spawnPos_vec3, spawnRot_q)
        table.insert(global_dynamicEntities_t, unit_u)
        stingray.Unit.set_data(unit_u, "Affected Unit", affectedUnit_s)
        stingray.Unit.set_data(unit_u, "pairID_i", pairID_i)
        
        empty_selected_hud_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "empty_selected",
            data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
        }
        scaleform.Stage.dispatch_event(empty_selected_hud_event)
        
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_place_object")
	    end
        
    end
end--End of PlaceLever()

function InteractableObjects:SaveAndChange()
    local unit_u = stingray.World.unit_by_name(stingray.Application.main_world(), "TimeAnchor_End")
    local tAPos_vec3 = stingray.Unit.local_position(unit_u,1)
    local playerPos_vec3 = stingray.Unit.local_position(global_player_u,1)
    playerPos_vec3 = stingray.Vector3.subtract(tAPos_vec3, playerPos_vec3)
    local x, y, z = stingray.Quaternion.to_euler_angles_xyz(stingray.Unit.local_rotation(global_player_u,1))
    local rot_vec3 = stingray.Vector3(x,y,z)
    InternalValues.playerLoad.mapToLoad_i = InternalValues.playerLoad.mapToLoad_i + 1
    local currentLevel_i = InternalValues.playerLoad.mapToLoad_i
    SaveGame(global_inventoryManager_o, playerPos_vec3, rot_vec3 , currentLevel_i)
    
    SimpleProject.change_level(levelNames[currentLevel_i])
end


--If the player aims at a generator and is holding a battery, a battery will be placed in the generator.
--@Param unit_u, the unit that is being aimed on and to be interacted with
function InteractableObjects:PlacePower(unit_u)
    local isPlacedOne_b = stingray.Unit.get_data(unit_u, "Placed_one_b")
    local isPlacedTwo_b = stingray.Unit.get_data(unit_u, "Placed_two_b")
    local isPlacedThree_b = stingray.Unit.get_data(unit_u, "Placed_three_b")
    local isPlacedFour_b = stingray.Unit.get_data(unit_u, "Placed_four_b")
    local done_b = stingray.Unit.get_data(unit_u, "done_b")
    local spawnpoint_s = stingray.Unit.get_data(unit_u, "spawnpoint")
    local affected_s = stingray.Unit.get_data(unit_u, "Affected Unit")

    local affectedUnit_u = stingray.World.unit_by_name(stingray.Application.main_world(), spawnpoint_s)
    if(spawnpoint_s == "default" or affectedUnit_u == nil) then
        return
    end
    --local affectedUnitType_s = stingray.Unit.get_data(affectedUnit_u, "Type")
    local positionOne_node = stingray.Unit.node(unit_u, "PowerSource1")
    local positionTwo_node = stingray.Unit.node(unit_u, "PowerSource2")
    local positionThree_node = stingray.Unit.node(unit_u, "PowerSource3")
    local positionFour_node = stingray.Unit.node(unit_u, "PowerSource4")
    
    local genScale_vec3 = stingray.Unit.local_scale(unit_u, 1)
    local genRot_q = stingray.Unit.local_rotation(unit_u, 1)
    local spawnPosOne_vec3 = stingray.Unit.world_position(unit_u, positionOne_node) + stingray.Vector3(0, 0, 0.075)
    local spawnPosTwo_vec3 = stingray.Unit.world_position(unit_u, positionTwo_node) + stingray.Vector3(0, 0, 0.075)
    local spawnPosThree_vec3 = stingray.Unit.world_position(unit_u, positionThree_node) + stingray.Vector3(0, 0, 0.075)
    local spawnPosFour_vec3 = stingray.Unit.world_position(unit_u, positionFour_node) + stingray.Vector3(0, 0, 0.075)
    
    local levelSpawnPoint_u = stingray.World.unit_by_name(stingray.Application.main_world(), "room_wp")
    local levelSpawnPosition = stingray.Unit.world_position(levelSpawnPoint_u, 1)
    
    if(stingray.Unit.get_data(unit_u, "done_b") and not isPlacedFour_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Battery") then
        local pwrSrc_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", spawnPosFour_vec3)
        table.insert(global_dynamicEntities_t, pwrSrc_u)
        stingray.Unit.disable_physics(pwrSrc_u)
        stingray.Unit.set_local_rotation(pwrSrc_u, 1, genRot_q)
        stingray.Unit.set_local_scale(pwrSrc_u, 1, genScale_vec3)
        
        stingray.Unit.set_data(unit_u, "pwrSrcFour_u", pwrSrc_u)
        stingray.Unit.set_data(unit_u, "Placed_four_b", true)
        global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
        
        local affectedUnit_u = stingray.World.unit_by_name(stingray.Application.main_world(), affected_s)
        stingray.Unit.set_data(affectedUnit_u, "Placed", true)
        local affectedDungeon_s = stingray.Unit.get_data(affectedUnit_u, "Affected Unit")
        local affectedRuins_s = stingray.Unit.get_data(affectedUnit_u, "Affected Unit T2")
        local dungeonUnit_u = stingray.World.unit_by_name(stingray.Application.main_world(), affectedDungeon_s)
        local ruinsUnit_u = stingray.World.unit_by_name(stingray.Application.main_world(), affectedRuins_s)
        
        stingray.Unit.set_data(dungeonUnit_u, "affected_b", true)
        stingray.Unit.set_data(ruinsUnit_u, "affected_b", true)

        empty_selected_hud_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "empty_selected",
            data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
        }
        scaleform.Stage.dispatch_event(empty_selected_hud_event)
        return
    end
    
    --global_inventoryManager_o comes from project.lua. Script is Inventory.lua.
    if(not stingray.Unit.get_data(unit_u, "AllPlaced_b")) then
        if(not isPlacedOne_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Battery") then
            local pwrSrc_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", spawnPosOne_vec3)
            table.insert(global_dynamicEntities_t, pwrSrc_u)
            stingray.Unit.disable_physics(pwrSrc_u)
            stingray.Unit.set_local_rotation(pwrSrc_u, 1, genRot_q)
            stingray.Unit.set_local_scale(pwrSrc_u, 1, genScale_vec3)
            
            stingray.Unit.set_data(unit_u, "pwrSrcOne_u", pwrSrc_u)
            stingray.Unit.set_data(unit_u, "Placed_one_b", true)
            global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
            
            selected_slot_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "selected_slot",
                data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
            }
            scaleform.Stage.dispatch_event(selected_slot_hud_event)
            if(not done_b) then
                roomOne = stingray.World.load_level(stingray.Application.main_world(), "content/_Assets/Levels/Level_4_Room_1",levelSpawnPosition)
            end
            
        elseif(not isPlacedTwo_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Battery") then
            local pwrSrc_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", spawnPosTwo_vec3)
            table.insert(global_dynamicEntities_t, pwrSrc_u)
            stingray.Unit.disable_physics(pwrSrc_u)
            stingray.Unit.set_local_rotation(pwrSrc_u, 1, genRot_q)
            stingray.Unit.set_local_scale(pwrSrc_u, 1, genScale_vec3)
            
            stingray.Unit.set_data(unit_u, "pwrSrcTwo_u", pwrSrc_u)
            stingray.Unit.set_data(unit_u, "Placed_two_b", true)
            global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
            stingray.Unit.set_local_position(affectedUnit_u, 1, stingray.Unit.world_position(affectedUnit_u, 1) + stingray.Vector3(3,0,0))
            
            empty_selected_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "empty_selected",
                data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
            }
            scaleform.Stage.dispatch_event(empty_selected_hud_event)
            if(not done_b) then
                stingray.World.destroy_level(stingray.Application.main_world(), roomOne)
                roomTwo = stingray.World.load_level(stingray.Application.main_world(), "content/_Assets/Levels/Level_4_Room_2", levelSpawnPosition)
            end
        elseif(not isPlacedThree_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Battery") then
            
            local pwrSrc_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", spawnPosThree_vec3)
            table.insert(global_dynamicEntities_t, pwrSrc_u)
            stingray.Unit.disable_physics(pwrSrc_u)
            stingray.Unit.set_local_rotation(pwrSrc_u, 1, genRot_q)
            stingray.Unit.set_local_scale(pwrSrc_u, 1, genScale_vec3)
            
            stingray.Unit.set_data(unit_u, "pwrSrcThree_u", pwrSrc_u)
            stingray.Unit.set_data(unit_u, "Placed_three_b", true)
            stingray.Unit.set_data(unit_u, "AllPlaced_b", true)
            stingray.Unit.set_data(unit_u, "done_b", true)
            global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
            stingray.Unit.set_local_position(affectedUnit_u, 1, stingray.Unit.world_position(affectedUnit_u, 1) + stingray.Vector3(3,0,0))
            
            empty_selected_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "empty_selected",
                data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
            }
            scaleform.Stage.dispatch_event(empty_selected_hud_event)
            
            if(not done_b) then
                stingray.World.destroy_level(stingray.Application.main_world(), roomTwo)
                roomThree = stingray.World.load_level(stingray.Application.main_world(), "content/_Assets/Levels/Level_4_Room_3", levelSpawnPosition)
            end
        end--May be changed to outcommented block 
    else
        if(isPlacedOne_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == nil) then
            local pwrSrc_u = stingray.Unit.get_data(unit_u, "pwrSrcOne_u")
            global_inventoryManager_o:InsertToSelected("Battery", pwrSrc_u)
            
            stingray.Unit.set_data(unit_u, "pwrSrcOne_u", nil)
            stingray.Unit.set_data(unit_u, "Placed_one_b", false)

        elseif(isPlacedTwo_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == nil) then
            local pwrSrc_u = stingray.Unit.get_data(unit_u, "pwrSrcTwo_u")
            global_inventoryManager_o:InsertToSelected("Battery", pwrSrc_u)
            stingray.Unit.set_data(unit_u, "pwrSrcTwo_u", nil)
            stingray.Unit.set_data(unit_u, "Placed_two_b", false)

        elseif(isPlacedThree_b and global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == nil) then
            local pwrSrc_u = stingray.Unit.get_data(unit_u, "pwrSrcThree_u")
            global_inventoryManager_o:InsertToSelected("Battery", pwrSrc_u)
            stingray.Unit.set_data(unit_u, "pwrSrcThree_u", nil)
            stingray.Unit.set_data(unit_u, "Placed_three_b", false)
            stingray.Unit.set_data(unit_u, "AllPlaced_b", false)
            stingray.Unit.set_local_position(affectedUnit_u, 1, stingray.Unit.world_position(affectedUnit_u, 1) + stingray.Vector3(-3,0,0))

        end
    end
end--End of PlaceBattery()

function InteractableObjects:TwoPanelPlace(unit_u)
    local isPlacedOne_b = stingray.Unit.get_data(unit_u, "isPlacedOne_b")
    
    if (isPlacedOne_b == true) then
        if (global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Battery") then
            global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
            empty_selected_hud_event = 
            {
                eventId = scaleform.EventTypes.Custom,
                name = "empty_selected",
                data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
            }
            scaleform.Stage.dispatch_event(empty_selected_hud_event)
            local positionTwo_node = stingray.Unit.world_position(unit_u,stingray.Unit.node(unit_u, "Holder_2"))
            local rotation_q = stingray.Unit.local_rotation(unit_u, 1)
            local temp = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", positionTwo_node, rotation_q)
            table.insert(global_dynamicEntities_t, temp)
            stingray.Unit.disable_physics(temp)
            stingray.Unit.set_data(unit_u, "PS2", temp)
            stingray.Unit.set_data(unit_u, "Type", "done")
            local ifMassive_b = stingray.Unit.get_data(unit_u, "MassiveDoor")
            if (ifMassive_b) then
                local unitName_s = stingray.Unit.get_data(unit_u, "Affected Unit")
                local massiveDoor_u = stingray.World.unit_by_name(stingray.Application.main_world(), unitName_s)
                
                if(unitName_s == "default" or massiveDoor_u == nil) then
                    local affectedUnit_s = stingray.World.unit_by_name(stingray.Application.main_world(), "MassiveDoor")
                    stingray.Unit.animation_event(affectedUnit_s,"OpenDoor")
                    return
                else
                    stingray.Unit.set_data(massiveDoor_u, "inputThree_b")
                    return
                end
                
            end
            local affectedUnit_s = stingray.World.unit_by_name(stingray.Application.main_world(), "CombinationDoor")
            stingray.Unit.set_data(affectedUnit_s, "Activated_b", true)
            return
        end
    end
    if (global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] == "Battery") then
        global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i] = nil
        empty_selected_hud_event = 
        {
            eventId = scaleform.EventTypes.Custom,
            name = "empty_selected",
            data = {selectedSlot = global_inventoryManager_o.selectedSlot_i, itemInSlot = global_inventoryManager_o.inventorySlots_t[global_inventoryManager_o.selectedSlot_i]}
        }
        scaleform.Stage.dispatch_event(empty_selected_hud_event)
        local positionOne_node = stingray.Unit.world_position(unit_u, stingray.Unit.node(unit_u, "Holder_1"))
        local rotation_q = stingray.Unit.local_rotation(unit_u, 1)
        local temp = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", positionOne_node, rotation_q)
        table.insert(global_dynamicEntities_t, temp)
        stingray.Unit.disable_physics(temp)
        stingray.Unit.set_data(unit_u, "PS1", temp)
        stingray.Unit.set_data(unit_u, "isPlacedOne_b", true)
end
end

function InteractableObjects:CombinationWheel(unit_u)
    local world_w = stingray.Application.main_world()
    local comboDoor_u = world_w:unit_by_name("CombinationDoor")
    local activated_b = stingray.Unit.get_data(comboDoor_u, "Activated_b")
    if not(activated_b) then
        return
    end 
    stingray.Unit.animation_event(unit_u, "rotare")
    
    local currentNumber_i = stingray.Unit.get_data(unit_u,"currentNumber_i")
    local combinationNumber_i = stingray.Unit.get_data(unit_u, "comboNumber_i")
    if (currentNumber_i == 3) then
        currentNumber_i = 0
    end
    
    currentNumber_i = currentNumber_i + 1

    stingray.Unit.set_data(unit_u, "currentNumber_i",currentNumber_i)
    local wheelID_i = stingray.Unit.get_data(unit_u, "wheelID_i")

    if(combinationNumber_i == currentNumber_i) then
        if (wheelID_i == 1) then
            stingray.Unit.set_data(comboDoor_u, "WheelOne", true)
        elseif (wheelID_i == 2) then
            stingray.Unit.set_data(comboDoor_u, "WheelTwo", true)
        elseif (wheelID_i == 3) then
            stingray.Unit.set_data(comboDoor_u, "WheelThree", true)
        end
    else
        if (wheelID_i == 1) then
            stingray.Unit.set_data(comboDoor_u, "WheelOne", false)
        elseif (wheelID_i == 2) then
            stingray.Unit.set_data(comboDoor_u, "WheelTwo", false)
        elseif (wheelID_i == 3) then
            stingray.Unit.set_data(comboDoor_u, "WheelThree", false)
        end
    end
end

function InteractableObjects:PushPillar(unit_u)
end

return InteractableObjects