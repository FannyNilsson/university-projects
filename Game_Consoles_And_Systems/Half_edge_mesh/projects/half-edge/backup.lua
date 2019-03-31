require 'script/lua/FSM'
require 'script/lua/AStar'

CoalWorkers = 0
Craftsmen = 0
Discoverers = 0
-- define the class worker 
Worker = {}
Worker.__index = Worker

package.loaded[...] = Worker
local discoverer = require 'script/lua/Discoverer'
local craftyMan = require 'script/lua/Craftsman'
local coalDude = require 'script/lua/CoalWorker'

local bestPath = {}

function Worker:Create(Name, curNode, id)
    self.WorkerName = Name
    xPos = math.random(2,8)
    xPos = xPos/10
    
    yPos = math.random(2,8)
    yPos = yPos/10
    
    --the final pos for the man on the node
    finalXPos = curNode.x + xPos - 0.5
    finalYPos = curNode.y + yPos - 0.5

    local world = stingray.Application.main_world()
    local this = 
    {
        ID = id,
        stateMachine = FSM:Create(),
        inventoryFull = false,
        workerGraphic = stingray.World.spawn_unit(world, "core/units/primitives/cylinder_primitive", stingray.Vector3(finalXPos, finalYPos, 0)),
        Path = {},
        gNode,
        currentNode = curNode,
        HomeNode = curNode,
        timer = 0,
    }
    
    stingray.Unit.set_local_scale(this.workerGraphic, 1, stingray.Vector3(0.05,0.05,0.1))
    stingray.Unit.set_material(this.workerGraphic, "material", "content/materials/Pink")
    
     Idle =
        {
            Update = function(self, dt, me)
            --Check if there are any discoverers yet! Otherwise we need to upgrade!
            if Craftsmen < 1 then
                Craftsmen = Craftsmen + 1
                this.stateMachine:Change(7)
            end
            if Discoverers < 5 then
                Discoverers = Discoverers + 1
                this.stateMachine:Change(6)
            end
            if CoalWorkers < 2 then
                CoalWorkers = CoalWorkers + 1
                this.stateMachine:Change(8)
            end
            
            --Check a list which holds all discovered nodes with trees
            --If list is empty worker does nothing
            --Else changestate to FindTrees
            local inf = math.huge
            if #DiscoveredTreeNodes > 0 then
                this.gNode = DiscoveredTreeNodes[#DiscoveredTreeNodes]
                --ChangeState to FindTrees
                this.stateMachine:Change(2)
            end
            
            end,
            Enter = function()
            end,
            Exit = function() 
            end
        }
    
        FindTrees =
        {
            Update = function(self, dt, me) 
                if this.gNode == nil then
                    if #DiscoveredTreeNodes > 0 then
                        this.gNode = DiscoveredTreeNodes[#DiscoveredTreeNodes]
                        Worker:MoveUnit(this.workerGraphic, dt, me)
                        --ChangeState to FindTrees
                        this.stateMachine:Change(3)
                    end
                end
                Worker:MoveUnit(this.workerGraphic, dt, me)
                if this.currentNode == this.gNode then
                    this.stateMachine:Change(3)
                end
                
            end,
            Enter = function()
            end,
            Exit = function() 
            end
        }
    
        ChopTree =
        {
            Update = function(self, dt, me)
                if this.currentNode.Type == "TreeNode" and this.currentNode.Trees ~= nil then
                    
                    if #this.currentNode.Trees > 0 then
                        --only if inventory is not full
                        if this.inventoryFull == false then
                            stingray.World.destroy_unit(stingray.Application.main_world(), this.currentNode.Trees[#this.currentNode.Trees])
                            --stingray.Unit.set_unit_visibility(this.currentNode.Trees[#this.currentNode.Trees], false)
                            table.remove(this.currentNode.Trees)
                            print("tree removed")
                            --if the list of trees is empty on this node we will remove the node from the discoveredtreenodes-list
                            --if #this.currentNode.Trees == 0 then
                                --table.remove()
                            --end
                            this.inventoryFull = true
                            this.currentNode = this.gNode
                            this.gNode = this.HomeNode
                            bestPath[this.ID] = nil
                            this.stateMachine:Change(4)
                        end
                        if this.inventoryFull == true then
                            this.currentNode = this.gNode
                            this.gNode = this.HomeNode
                            bestPath[this.ID] = nil
                            this.stateMachine:Change(4)
                        end
                    end
                end
            if this.currentNode.Type == "TreeNode" and this.currentNode.Trees == nil then
                this.stateMachine:Change(4)
            end
            end,
            Enter = function() 
            end,
            Exit = function() 
            end
        }
    
        GoHome =
        {
            Update = function(self, dt, me)
               this.gNode = this.HomeNode
               bestPath[this.ID] = nil
               Worker:MoveUnit(this.workerGraphic, dt, me)
               
                --Release the tree at the inventory
                if this.inventoryFull == true then
                    InventoryForAll = InventoryForAll + 1
                    this.inventoryFull = false
                    print(InventoryForAll)
                end
                this.stateMachine:Change(1)
            end,
            Enter = function() 
            end,
            Exit = function() 
            end
        }
    
        Suicide =
        {
            Update = function(self, dt, me)
                --hide the agent or delete
                --stingray.World.destroy_unit(stingray.Application.main_world(), this.workerGraphic)
                stingray.Unit.set_unit_visibility(this.workerGraphic, false)
            end,
            Enter = function() 
            end,
            Exit = function() 
            end
        }
    
        UpgradeDisc =
        {
            Update = function(self, dt, me)
                this.timer = this.timer + 1
                if this.timer >= 120 then
                    Worker:CreateDiscoverer(this.currentNode)
                    --reset the timer
                    this.timer = 0
                    this.stateMachine:Change(5)
                end
                
            end,
            Enter = function() 
            end,
            Exit = function() 
            end
        }
    
        UpgradeCrafts =
        {
            Update = function(self, dt)
                this.timer = this.timer + 1
                if this.timer >= 120 then
                    Worker:CreateCraftsman(this.currentNode)
                    this.timer = 0
                    this.stateMachine:Change(5)
                end
            end,
            Enter = function() 
            end,
            Exit = function() 
            end
        }

        UpgradeCoal =
        {
            Update = function(self, dt)
                this.timer = this.timer + 1
                if this.timer >= 120 then
                    Worker:CreateCoalWorker(this.currentNode)
                    this.timer = 0
                    this.stateMachine:Change(5)
                end
            end,
            Enter = function() 
            end,
            Exit = function() 
            end
        }
    
    --Add the states to the FSM
    this.stateMachine:Add(1, Idle)
    this.stateMachine:Add(2, FindTrees)
    this.stateMachine:Add(3, ChopTree)
    this.stateMachine:Add(4, GoHome)
    this.stateMachine:Add(5, Suicide)
    this.stateMachine:Add(6, UpgradeDisc)
    this.stateMachine:Add(7, UpgradeCrafts)
    this.stateMachine:Add(8, UpgradeCoal)
    
    this.stateMachine.Current = Idle
    
    return this
end

function Worker:astar(me)
    --something went wrong with the gNode. Go back to the state and fix it
    if me.gNode == nil then
        return
    end
    if listOfDiscoveredNodes == nil then
        return
    end
    bestPath[me.ID] = AStar.AStar(me.currentNode, me.gNode, listOfDiscoveredNodes)
end

function Worker:CreateDiscoverer(startnode)
    local man 
    man = discoverer:Create(startnode, #listOfDisc+1)
    listOfDisc[#listOfDisc+1] = man
end

function Worker:CreateCraftsman(startnode)
    local man
    man = craftyMan:Create("Jerry", startnode)
    listOfCrafts[#listOfCrafts+1] = man
end

function Worker:CreateCoalWorker(startnode)
    local man
    man = coalDude:Create("Fia-Lotta", startnode)
    listOfCoalw[#listOfCoalw+1] = man
end

function Worker:MoveUnit(unit, dt, me)

    if bestPath[me.ID] == nil then
        print("gris")
        Worker:astar(me)
    end
    
    --if the list is empty and the worker wants to go home we call astar()
    if #bestPath[me.ID] == 0 then
        Worker:astar(me)
    end
    
    local currentindex = #bestPath[me.ID]
    
    if currentindex == 1 and me.inventoryFull == false then
        me.currentNode = me.gNode
        return
    end
    
    if currentindex == 1 and me.inventoryFull == true then
        me.currentNode = me.gNode
        return
    end
    
    
    if (bestPath[me.ID] ~= nil) then
        print("bjas")
        if #bestPath[me.ID] == 0 then
            Worker:astar(me)
        end
        local diff = stingray.Vector3(bestPath[me.ID][#bestPath[me.ID]].x, bestPath[me.ID][#bestPath[me.ID]].y, 0) - stingray.Unit.world_position(unit, 1)
        local direction = stingray.Vector3.normalize(diff)
        local speed = direction * 5 * dt
        print("bajsss")
        if stingray.Vector3.length(speed) >= stingray.Vector3.length(diff) then
            stingray.Unit.set_local_position(unit, 1, stingray.Vector3(bestPath[me.ID][#bestPath[me.ID]].x, bestPath[me.ID][#bestPath[me.ID]].y, 0))
            if #bestPath > 2 then
                me.currentNode = bestPath[me.ID][currentindex]
                --update the postition of our worker
                table.remove(bestPath[me.ID], #bestPath[me.ID])
                
            end
            
        else
            stingray.Unit.set_local_position(unit, 1, speed + stingray.Unit.world_position(unit, 1))
        end
    end
end
