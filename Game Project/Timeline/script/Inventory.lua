require 'core/appkit/lua/class'
require 'core/appkit/lua/app'
local InternalValues = InternalValues or require "script/lua/agentInternalData"																			   
local ComponentManager = require 'core/appkit/lua/component_manager'
Inventory = Appkit.class(Inventory)
package.loaded[...] = Inventory

local Wwise = stingray.Wwise
local WwiseWorld = stingray.WwiseWorld
local wwise_world = nil

local eventTypes = scaleform.EventTypes

--Initializes the class 
function Inventory:init()
    self.inventorySlots_t = {}
    self.itemSubTypes_t = {}
    self.selectedSlot_i = 1
    self.maxSize_i = 4
end--End of init()

--Changes which slot is selected in the inventpory
--@Param slotIndex_i, index of the slot the player is changing to, 0 < slotIndex_i < 5
function Inventory:ChangeSelected(slotIndex_i)
    if(slotIndex_i > self.maxSize_i) then
        self.selectedSlot_i = 1
    elseif(slotIndex_i < 1) then
        self.selectedSlot_i = self.maxSize_i
    else
        self.selectedSlot_i = slotIndex_i
    end
    
    local selected_slot_hud_event = nil
    selected_slot_hud_event = 
    {
        eventId = scaleform.EventTypes.Custom,
        name = "selected_slot",
        data = {selectedSlot = self.selectedSlot_i, itemInSlot = self.inventorySlots_t[self.selectedSlot_i]}
    }
    scaleform.Stage.dispatch_event(selected_slot_hud_event)
end--End of ChangeSelected()

function Inventory:LoadInventory()
    self.inventorySlots_t[1] = InternalValues.playerLoad.inventorySlot1_s
    self.inventorySlots_t[2] = InternalValues.playerLoad.inventorySlot2_s
    self.inventorySlots_t[3] = InternalValues.playerLoad.inventorySlot3_s
    self.inventorySlots_t[4] = InternalValues.playerLoad.inventorySlot4_s
    
    self.itemSubTypes_t[1] = InternalValues.playerLoad.itemSubTypeSlot1_i
    self.itemSubTypes_t[2] = InternalValues.playerLoad.itemSubTypeSlot2_i
    self.itemSubTypes_t[3] = InternalValues.playerLoad.itemSubTypeSlot3_i
    self.itemSubTypes_t[4] = InternalValues.playerLoad.itemSubTypeSlot4_i

end

--Drops the item in the selected slot infront of the player.										  
function Inventory:DropItem()
    local itemType_s = self:GetSelectedType()
    local empty_selected_hud_event
    
    --Converts the center of screen position to a world position right infront of the player
    local width_f, height_f = stingray.Application.back_buffer_size()
    local screenPosX_f = math.floor(width_f / 2)     
    local screenPosY_f = math.floor(height_f / 2)
    local spawnPos_vec3 = stingray.Camera.screen_to_world(stingray.Unit.camera(SimpleProject.camera_unit, 1), stingray.Vector3(screenPosX_f, screenPosY_f, 0), 1)
    local camRot_q = stingray.Unit.world_rotation(SimpleProject.camera_unit, 1)
    
    local anglVel_vec3 = stingray.Quaternion.forward(camRot_q) + spawnPos_vec3
    local velocity_vec3 = stingray.Vector3.multiply_elements(stingray.Quaternion.forward(camRot_q), stingray.Vector3(1, 1, 1))
    
    --Spawns a unit of the selected type and adds a velocity to its physics actor
    if(itemType_s == "Key") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Keys/Geo_Key_1_Placeholder", spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "Key_1")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
    end
    if(itemType_s == "Rock") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Environment/Rocks/Geo_Rock"..self.itemSubTypes_t[self.selectedSlot_i], spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "pCube"..self.itemSubTypes_t[self.selectedSlot_i])
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
    end
    if(itemType_s == "Seeds") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Seeds/Geo_Seed", spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "pSphere1_ncl1_1")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
    end
    if(itemType_s == "Critter") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Character/Critter/Geo_Critter_throwable", spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "actor")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
        stingray.Unit.set_data(dropped_u, "thrownInDungeon_b", global_inDungeon_b)
    end
    if(itemType_s == "Cogwheel") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Cogs/Geo_Cog_"..self.itemSubTypes_t[self.selectedSlot_i].."_Placeholder", spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "gear1")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
    end
    if(itemType_s == "Battery") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_"..self.itemSubTypes_t[self.selectedSlot_i], spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "pSphere6")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
    end
    if(itemType_s == "Loose Lever") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Levers/Geo_LOOSE_Lever_Placeholder", spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "pCylinder3")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
    end
    self.inventorySlots_t[self.selectedSlot_i] = nil
    empty_selected_hud_event = 
    {
        eventId = scaleform.EventTypes.Custom,
        name = "empty_selected",
        data = {selectedSlot = self.selectedSlot_i, itemInSlot = self.inventorySlots_t[self.selectedSlot_i]}
    }
    scaleform.Stage.dispatch_event(empty_selected_hud_event)
end--End of EmptySelected()

--gets the type of the currently selected item
function Inventory:GetSelectedType()
    return self.inventorySlots_t[self.selectedSlot_i]
end--End of GetSelectedType()

--inserts a string "reference" into the selected slot, and despawns the aimed at unit
--@Param insertObject_s, string of the item type, i.e "Key" or "Rock"
--@Param unit, the unit itself
function Inventory:InsertToSelected(insertObject_s, unit_u)
    local end_interact_hud_event = nil
    local insert_selected_hud_event = nil
    for i = 1, 5 do
        if (i > 4) then
            i = 1
        end
        if (self.inventorySlots_t[self.selectedSlot_i] == nil) then
            self.inventorySlots_t[self.selectedSlot_i] = insertObject_s
            self.itemSubTypes_t[self.selectedSlot_i] = stingray.Unit.get_data(unit_u, "subType_i")
            stingray.Unit.set_data(unit_u, "despawn_b", true)
            end_interact_hud_event =
            {
                eventId = scaleform.EventTypes.Custom,
                name = "end_interaction",
                data = {interactionType = nil, visible = false}
            }
            scaleform.Stage.dispatch_event(end_interact_hud_event)
            break
        elseif(self.inventorySlots_t[i] == nil) then
            self.inventorySlots_t[i] = insertObject_s
            self.itemSubTypes_t[i] = stingray.Unit.get_data(unit_u, "subType_i")
            stingray.Unit.set_data(unit_u, "despawn_b", true)
            self:ChangeSelected(i)
            end_interact_hud_event =
            {
                eventId = scaleform.EventTypes.Custom,
                name = "end_interaction",
                data = {interactionType = nil, visible = false}
            }
            scaleform.Stage.dispatch_event(end_interact_hud_event)
            break
        end
        i = i + 1
    end
    insert_selected_hud_event = 
    {
        eventId = scaleform.EventTypes.Custom,
        name = "insert_selected",
        data = {selectedSlot = self.selectedSlot_i, itemInSlot = self.inventorySlots_t[self.selectedSlot_i]}
    }
    scaleform.Stage.dispatch_event(insert_selected_hud_event)
end--End of InsertToSelected()

-- Throws the selected object from the inventory
function Inventory:ThrowSelected()
    local itemType_s = self:GetSelectedType()
    
    --Converts the center of screen position to a world position right infront of the player
    local width_f, height_f = stingray.Application.back_buffer_size()
    local screenPosX_f = math.floor(width_f / 2)     
    local screenPosY_f = math.floor(height_f / 2)
    local spawnPos_vec3 = stingray.Camera.screen_to_world(stingray.Unit.camera(SimpleProject.camera_unit, 1), stingray.Vector3(screenPosX_f, screenPosY_f, 0), 1)
    local camRot_q = stingray.Unit.world_rotation(SimpleProject.camera_unit, 1)
    local anglVel_vec3 = stingray.Quaternion.forward(camRot_q) + spawnPos_vec3
    local velocity_vec3 = stingray.Vector3.multiply_elements(stingray.Quaternion.forward(camRot_q), stingray.Vector3(25, 25, 25))
    --Spawns a unit of the selected type and adds a velocity to its physics actor
    if(itemType_s == "Key") then
        local key_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Keys/Geo_Key_1_Placeholder", spawnPos_vec3)
        local keyUnit_a = stingray.Unit.actor(key_u, "Key_1")
        stingray.Actor.set_velocity(keyUnit_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(keyUnit_a, anglVel_vec3)
        
        self.inventorySlots_t[self.selectedSlot_i] = nil
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_throw_object")
	    end
    end
    if(itemType_s == "Rock") then
        --local rock_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Environment/Rocks/Throwable_Rock3", spawnPos_vec3)
        local rock_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Environment/Rocks/Throwable_Rock"..self.itemSubTypes_t[self.selectedSlot_i], spawnPos_vec3)
        local rockUnit_a = stingray.Unit.actor(rock_u, 1)
        
        stingray.Actor.set_velocity(rockUnit_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(rockUnit_a, anglVel_vec3)
        
        self.inventorySlots_t[self.selectedSlot_i] = nil
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_throw_object")
	    end
    end
    if(itemType_s == "Seeds") then
        local seed_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Seeds/Geo_Seed", spawnPos_vec3)
        local seedUnit_a = stingray.Unit.actor(seed_u, "pSphere1_ncl1_1")
        stingray.Actor.set_velocity(seedUnit_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(seedUnit_a, anglVel_vec3)
        
        self.inventorySlots_t[self.selectedSlot_i] = nil
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_throw_object")
	    end
    end
    if(itemType_s == "Critter") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Character/Critter/Geo_Critter_throwable", spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "actor")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
        stingray.Unit.set_data(dropped_u, "thrownInDungeon_b", global_inDungeon_b)
        stingray.Actor.add_impulse(dropped_a, stingray.Vector3(1, 0.3, -0.2))
        
        self.inventorySlots_t[self.selectedSlot_i] = nil
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_throw_object")
	    end
    end
    if(itemType_s == "Cogwheel") then
        math.randomseed(os.clock())
        local oneTwoThree = math.random(1, 3)
        local cWheel_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Cogs/Geo_Cog_"..self.itemSubTypes_t[self.selectedSlot_i].."_Placeholder", spawnPos_vec3)
        local cWheelUnit_a = stingray.Unit.actor(cWheel_u, "gear1")
        stingray.Actor.set_velocity(cWheelUnit_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(cWheelUnit_a, anglVel_vec3)
        
        self.inventorySlots_t[self.selectedSlot_i] = nil
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_throw_object")
	    end
    end
    if(itemType_s == "Battery") then
        local cWheel_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_"..self.itemSubTypes_t[self.selectedSlot_i], spawnPos_vec3)
        local cWheelUnit_a = stingray.Unit.actor(cWheel_u, "pSphere6")
        stingray.Actor.set_velocity(cWheelUnit_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(cWheelUnit_a, anglVel_vec3)
        
        self.inventorySlots_t[self.selectedSlot_i] = nil
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_throw_object")
	    end
    end
    if(itemType_s == "Loose Lever") then
        local dropped_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Levers/Geo_LOOSE_Lever_Placeholder", spawnPos_vec3)
        local dropped_a = stingray.Unit.actor(dropped_u, "pCylinder3")
        stingray.Actor.set_velocity(dropped_a, velocity_vec3)
        stingray.Actor.set_angular_velocity(dropped_a, anglVel_vec3)
        
        self.inventorySlots_t[self.selectedSlot_i] = nil
        if Wwise then
		    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
		    WwiseWorld.trigger_event(wwise_world, "sfx_throw_object")
	    end
    end
    empty_selected_hud_event = 
    {
        eventId = scaleform.EventTypes.Custom,
        name = "empty_selected",
        data = {selectedSlot = self.selectedSlot_i, itemInSlot = self.inventorySlots_t[self.selectedSlot_i]}
    }
    scaleform.Stage.dispatch_event(empty_selected_hud_event)
    
    
end--end of ThrowSelected()

--Checks if there has been any input that the inventory needs to respond to
--@Param dt. delta time, time since last frame
function Inventory:Update(dt)
    if Appkit.input_mapper:SlotOne() then
        self:ChangeSelected(1)
        self:GetSelectedType()
    end
    if Appkit.input_mapper:SlotTwo() then
        self:ChangeSelected(2)
        self:GetSelectedType()
    end
    if Appkit.input_mapper:SlotThree() then
        self:ChangeSelected(3)
        self:GetSelectedType()
    end
    if Appkit.input_mapper:SlotFour() then
        self:ChangeSelected(4)
        self:GetSelectedType()
    end
    if Appkit.input_mapper:DropItem() then
        self:DropItem()
    end
    if Appkit.input_mapper:ScrollUp() then
        self:ChangeSelected(self.selectedSlot_i + 1)
    end
    if Appkit.input_mapper:ScrollDown() then
        self:ChangeSelected(self.selectedSlot_i - 1)
    end
    if Appkit.input_mapper:ThrowItem() then
        self:ThrowSelected()
    end
end--End of Update()

return Inventory