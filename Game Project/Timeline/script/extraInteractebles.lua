
local openingComboDoor = false
local twoSourceHolder_u = nil
local plate1_u = nil
local plate2_u = nil
local plate3_u = nil
local doorTimer_f = nil




function StartGeneratorActivated(unit_u)
    local isPlaced_b = stingray.Unit.get_data(unit_u, "Placed")
    
    --local affectedUnitType_s = stingray.Unit.get_data(affectedUnit_u, "Type")
    local position_node = stingray.Unit.node(unit_u, "position")
    local genScale_vec3 = stingray.Unit.local_scale(unit_u, 1)
    local genRot_q = stingray.Unit.local_rotation(unit_u, 1)
    local spawnPos_vec3 = stingray.Unit.world_position(unit_u, position_node) + stingray.Vector3(0, 0, 0.075)
    
        
    local pwrSrc_u = stingray.World.spawn_unit(stingray.Application.main_world(), "content/_Assets/Key Items/Power Source/Geo_PowerSource_Placeholder_1", spawnPos_vec3)
    stingray.Unit.disable_physics(pwrSrc_u)
    stingray.Unit.set_local_rotation(pwrSrc_u, 1, genRot_q)
    stingray.Unit.set_local_scale(pwrSrc_u, 1, genScale_vec3)
    
    stingray.Unit.set_data(unit_u, "pwrSrc_u", pwrSrc_u)
    stingray.Unit.set_data(unit_u, "Placed", true)
    
    stingray.Unit.animation_event(unit_u, "Activate")
    table.insert(global_dynamicEntities_t, pwrSrc_u)
end

function SetupCombinationDoorLevel3()
    
    local world_w = stingray.Application.main_world()
    local comboDoor_u = world_w:unit_by_name("CombinationDoor")
    local genRot_q = stingray.Unit.local_rotation(comboDoor_u, 1)

    local pos1_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Plate_Location1"))
    local pos2_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Plate_Location2"))
    local pos3_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Plate_Location3"))
    local pos2Source_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Two_Source_Holder"))
    
    pos1_v3 = pos1_v3 + stingray.Vector3(-0.04,0,0)
    pos2_v3 = pos2_v3 + stingray.Vector3(-0.04,0,0)
    pos3_v3 = pos3_v3 + stingray.Vector3(-0.04,0,0)
    pos2Source_v3 = pos2Source_v3 + stingray.Vector3(-0.05,0,0)
    
    plate1_u = world_w:spawn_unit("content/_Assets/Environment/Combination_Door/CombinationDoor_Rotator", pos1_v3)
    plate2_u = world_w:spawn_unit("content/_Assets/Environment/Combination_Door/CombinationDoor_Rotator", pos2_v3)

    plate3_u = world_w:spawn_unit("content/_Assets/Environment/Combination_Door/CombinationDoor_Rotator", pos3_v3)
    twoSourceHolder_u = world_w:spawn_unit("content/_Assets/Key Items/2_Slot_Source/Geo_2slotSource_Placeholder", pos2Source_v3)
    
    stingray.Unit.set_data(plate1_u, "wheelID_i", 1)
    stingray.Unit.set_data(plate2_u, "wheelID_i", 2)
    stingray.Unit.set_data(plate3_u, "wheelID_i", 3)
    
    stingray.Unit.animation_event(plate2_u, "rotare")
    
    stingray.Unit.set_data(plate1_u, "comboNumber_i", 3)
    stingray.Unit.set_data(plate2_u, "comboNumber_i", 3)
    stingray.Unit.set_data(plate3_u, "comboNumber_i", 2)
    
    stingray.Unit.set_local_rotation(plate1_u, 1, genRot_q)
    stingray.Unit.set_local_rotation(plate2_u, 1, genRot_q)
    stingray.Unit.set_local_rotation(plate3_u, 1, genRot_q)
    stingray.Unit.set_local_rotation(twoSourceHolder_u, 1, genRot_q)
    
    
    table.insert(global_dynamicEntities_t, plate1_u)
    table.insert(global_dynamicEntities_t, plate2_u)
    table.insert(global_dynamicEntities_t, plate3_u)
    table.insert(global_dynamicEntities_t, twoSourceHolder_u)
end

function ComboDoorOpen()
    doorTimer_f = stingray.Application.time_since_launch()
    openingComboDoor = true
    local world_w = stingray.Application.main_world()
    local comboDoor_u = world_w:unit_by_name("CombinationDoor")
    stingray.Unit.animation_event(comboDoor_u, "open")
end


function ComboDoorUpdate()
    
    if(openingComboDoor) then
        local world_w = stingray.Application.main_world()
        local comboDoor_u = world_w:unit_by_name("CombinationDoor")
        local ps1_u = stingray.Unit.get_data(twoSourceHolder_u, "PS1")
        local ps2_u = stingray.Unit.get_data(twoSourceHolder_u, "PS2")
                
        local pos1_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Plate_Location1"))
        local pos2_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Plate_Location2"))
        local pos3_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Plate_Location3"))
        local pos2Source_v3 = stingray.Unit.world_position(comboDoor_u, stingray.Unit.node(comboDoor_u,"Two_Source_Holder"))
        local ps1_v3 = stingray.Unit.world_position(twoSourceHolder_u,stingray.Unit.node(twoSourceHolder_u, "Holder_1"))
        local ps2_v3 = stingray.Unit.world_position(twoSourceHolder_u,stingray.Unit.node(twoSourceHolder_u, "Holder_2"))


        pos1_v3 = pos1_v3 + stingray.Vector3(-0.04,0,0)
        pos2_v3 = pos2_v3 + stingray.Vector3(-0.04,0,0)
        pos3_v3 = pos3_v3 + stingray.Vector3(-0.04,0,0)
        pos2Source_v3 = pos2Source_v3 + stingray.Vector3(-0.05,0,0)
        
        stingray.Unit.set_local_position(plate1_u,1 ,pos1_v3)
        stingray.Unit.set_local_position(plate2_u,1 ,pos2_v3)
        stingray.Unit.set_local_position(plate3_u,1 ,pos3_v3)
        stingray.Unit.set_local_position(twoSourceHolder_u,1 ,pos2Source_v3)
        stingray.Unit.set_local_position(ps1_u,1 ,ps1_v3)
        stingray.Unit.set_local_position(ps2_u,1 ,ps2_v3)
        
        
        local tempTime = stingray.Application.time_since_launch() - doorTimer_f
        if (tempTime > 5) then
            openingComboDoor = false
        end
    end
end