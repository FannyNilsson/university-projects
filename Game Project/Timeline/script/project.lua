-----------------------------------------------------------------------------------
-- This implementation uses the default SimpleProject and the Project extensions are 
-- used to extend the SimpleProject behavior.

-- This is the global table name used by Appkit Basic project to extend behavior
Project = Project or {}

local InternalValues = InternalValues or require "script/lua/agentInternalData"
require 'script/lua/flow_callbacks'
require 'script/lua/Inventory'
require 'script/lua/load_save'
require 'script/lua/input_mapper'
require 'script/lua/pause_menu'

local Wwise = stingray.Wwise
local WwiseWorld = stingray.WwiseWorld
local world_w = stingray.Application.main_world()

local timeshiftSafety_u
local levelNumber_f = 1
global_timeshiftSafety_u = nil
global_dynamicEntities_t = {}
global_EndGame = false
global_EndCredits = true
global_playedDungeonCutscene = false

local timeAnchorAudio_w = nil
local played_b = false

global_gameIsPaused_b = false
global_inventoryManager_o = Inventory(stingray.Application.main_world())
local test_Win_b2 = true
global_winMusic_b = false

--PatrolState = PatrolState or require "script/lua/patrolState"
require 'script/lua/extraInteractebles'
require "script/lua/agentInternalData"
require 'script/lua/beast'
require "script/lua/dungeonKeeper"
require "script/lua/entityManager"
require "script/lua/debug"

Project.level_names = {
	menu = "content/levels/main_menu",
	level_0 = "content/_Assets/Levels/Intro_Dunkelberg_Level",
	level_1 = "content/_Assets/Levels/Level 1",
	level_2 = "content/_Assets/Levels/Level_2",
	level_3 = "content/_Assets/Levels/Level_3",
	level_4 = "content/_Assets/Levels/Level_4",
    test = "content/levels/character"
}


levelNames = {
    "content/levels/main_menu",
    "content/_Assets/Levels/Intro_Dunkelberg_Level",
    "content/_Assets/Levels/Level 1",
    "content/_Assets/Levels/Level_2",
    "content/_Assets/Levels/Level_3",
    "content/_Assets/Levels/Level_4",
    "content/levels/character"
}

global_playerDetected_b = false
global_entityManager_o = nil

global_changeAndSave_b = false

global_materialList_t = nil
local test = nil
local loadedTimer_f = 0
local exitTimer_f = 0
local level_name
-- Can provide a config for the basic project, or it will use a default if not.
local SimpleProject = require 'core/appkit/lua/simple_project'
SimpleProject.config = {
	standalone_init_level_name = Project.level_names.menu,
	camera_unit = "core/appkit/units/camera/camera",
	camera_index = 1,
	shading_environment = nil, -- Will override levels that have env set in editor.
	create_free_cam_player = false, -- Project will provide its own player.
	exit_standalone_with_esc_key = false
}

-- This optional function is called by SimpleProject after level, world and player is loaded 
-- but before lua trigger level loaded node is activated.
function Project.on_level_load_pre_flow()
    --stingray.Camera.set_far_range(SimpleProject.camera_unit,10)
	-- Spawn the player for all non-menu levels. level_name will be nil if this 
	-- is an unsaved Test Level.
	local world_w = stingray.Application.main_world()
	global_timeshiftSafety_u = stingray.World.unit_by_name(world_w, "Dunkel_Crouch_Unit")
	global_entityManager_o = EntityManager() -- need to be after spawn_player
    global_entityManager_o:InitiateAgents()
    
	global_materialList_t = {}
	global_inventoryManager_o:LoadInventory()
	
    level_name = SimpleProject.level_name
	if level_name == nil or level_name ~= Project.level_names.menu then
		local view_position = Appkit.editor_view_position or stingray.Vector3(0,-8,3)
		local view_rotation = Appkit.editor_view_rotation or stingray.Quaternion.identity()
		local Player = require 'script/lua/player'
		Player.spawn_player(SimpleProject.level, view_position, view_rotation)
		
		stingray.Unit.set_data(global_player_u, "red_b", false)
        stingray.Unit.set_data(global_player_u, "yellow_b", false)

	elseif level_name == Project.level_names.menu then
	    local MainMenu = require 'script/lua/main_menu'
	    MainMenu.start()
	end
	 
	if (level_name == Project.level_names.level_3) then
	    local generator1_u = world_w:unit_by_name("Generator_TimeShiftStop1")
        local generator2_u = world_w:unit_by_name("Generator_TimeShiftStop2")
        if (generator1_u ~= nil and generator2_u ~= nil) then
            StartGeneratorActivated(generator1_u)
            StartGeneratorActivated(generator2_u)
        end
        SetupCombinationDoorLevel3()
    end
    
	local tempList_t = stingray.World.units_by_resource(world_w, "content/_Assets/Environment/Floor/Floor_Mesh_Check")
    for k, unit_u in pairs(tempList_t) do
        local tempDataList_t = {
            matId_i = stingray.Material.id(stingray.Mesh.material(stingray.Unit.mesh(unit_u, 1), 1)),
            loudness_f = stingray.Unit.get_data(unit_u, "loudness_f"),
            name_s = stingray.Unit.get_data(unit_u, "name_s")
        }
        table.insert(global_materialList_t, tempDataList_t)
    end


end

function Project.on_level_shutdown_post_flow()
    if (global_entityManager_o ~= nil) then
        global_entityManager_o:Clear()
        global_entityManager = nil
    end
    if (global_dynamicEntities_t ~= nil) then
        local world_w = stingray.Application.main_world()
        for key, value in ipairs(global_dynamicEntities_t) do 
            if (stingray.Unit.alive(value)) then
                stingray.World.destroy_unit(world_w, value) 
            end
            global_dynamicEntities_t[key] = nil
        end
    end
end

--Function called by SimpleProject which sets the position and rotation of the "ghost unit to match the player with correct offset"
function Project.update(dt)
    --if the game isn't paused then all updates will be made
    if(not global_gameIsPaused_b) then
        if(level_name ~= Project.level_names.menu ) then
            --stingray.Unit.animation_event(global_player_u, "TimeShift")
            stingray.Unit.set_local_rotation(global_timeshiftSafety_u, 1, stingray.Unit.local_rotation(global_player_u, 1))
            stingray.Unit.set_local_position(global_timeshiftSafety_u, 1, stingray.Unit.world_position(global_player_u, 1)+ stingray.Vector3(stingray.Unit.get_data(global_timeshiftSafety_u, "xOffset_f"), stingray.Unit.get_data(global_timeshiftSafety_u, "yOffset_f"), stingray.Unit.get_data(global_timeshiftSafety_u, "zOffset_f")))
            global_inventoryManager_o:Update(dt)
        
            global_entityManager_o:Update(dt)
        end

        if(global_inventoryManager_o.inventorySlots_t[1] ~= nil and global_inventoryManager_o.inventorySlots_t[2] ~= nil and global_inventoryManager_o.inventorySlots_t[3] ~= nil and global_inventoryManager_o.inventorySlots_t[4] ~= nil) then
            test_Win_b = true
            if(test_Win_b and test_Win_b2) then
                --Project.testWin()
            end
        end
        
        if (global_changeAndSave_b) then
            local unit_u = stingray.World.unit_by_name(stingray.Application.main_world(), "TimeAnchor_End")
            local nextLevelBarrel_u =  stingray.World.unit_by_name(stingray.Application.main_world(), "Next_Level_Barrel")
            local nextLevel_i = stingray.Unit.get_data(nextLevelBarrel_u, "nextLevel_i")
            --play travel sound
            if(not played_b)then
                if Wwise then
                    wwise_world = wwise_world or Wwise.wwise_world(SimpleProject.world)
                    timeAnchorAudio_w = WwiseWorld.trigger_event(wwise_world, "sfx_time_anchor")
                end
                played_b = true
            end
            
            local tAPos_vec3 = stingray.Unit.local_position(unit_u,1)
            local playerPos_vec3 = stingray.Unit.local_position(global_player_u,1)
            playerPos_vec3 = stingray.Vector3.subtract(tAPos_vec3, playerPos_vec3)
            local x, y, z = stingray.Quaternion.to_euler_angles_xyz(stingray.Unit.local_rotation(global_player_u,1))
            local rot_vec3 = stingray.Vector3(x,y,z)
            SaveGame(global_inventoryManager_o, playerPos_vec3, rot_vec3 , nextLevel_i)
            SimpleProject.change_level(levelNames[nextLevel_i])
            global_changeAndSave_b = false
            
        end
        
        if (global_EndGame) then
            stingray.Unit.set_data(global_player_u, "inCutscene_b", true)
            stingray.Unit.set_data(global_player_u, "cutsceneName_s", "End")
        end
        ComboDoorUpdate()   
        CheckSaveFile()
        LoadUpdate()
        LoadSettingsUpdate()
        
        
    end
    
end

function Project.testWin()
    --LevelCompletedMenu.start()
    WinMenu.start()
    test_Win_b2 = false
end

-- Optional function called by SimpleProject *before* appkit/world render
function Project.render()
end

-- Optional function called by SimpleProject *before* appkit/level/player/world shutdown
function Project.shutdown()
    if (global_entityManager_o ~= nil) then
        global_entityManager_o:Clear()
        global_entityManager = nil
    end
end

return Project