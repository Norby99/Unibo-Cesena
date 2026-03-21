local SubsumptionController = {}
SubsumptionController.__index = SubsumptionController

--[[
    SubsumptionController is a behavior-based control system that manages multiple behaviors for
    a robot. Each behavior can propose actions for the left and right motors, and the controller
    selects the most appropriate action based on the behaviors' priorities.

    The controller iterates through the behaviors, checking if they propose valid actions. If a
    behavior proposes an action, it is selected immediately, and the controller returns the
    corresponding motor speeds. If no behaviors propose actions, it defaults to using the
    maximum velocity for both motors.
--]]
function SubsumptionController.new(max_velocity)
    local self = setmetatable({}, SubsumptionController)
    self.behaviors = {}
    self.max_velocity = max_velocity
    return self
end

-- Adds a behavior to the controller's list of behaviors.
function SubsumptionController:add(behavior)
    table.insert(self.behaviors, behavior)
end

--[[
    Runs the controller by iterating through the behaviors and checking for proposed actions.
    The first behavior that proposes valid actions for both left and right motors is selected,
    and its actions are returned. If no behaviors propose actions, the controller returns the
    default maximum velocity for both motors.
--]]
function SubsumptionController:run()
    local n = #self.behaviors
    local left_speed = self.max_velocity
    local right_speed = self.max_velocity
    local total_weight = 0

    for _, b in ipairs(self.behaviors) do
        local action_left, action_right = b:action()

        if action_left ~= nil and action_right ~= nil then
            log("[Controller] Behavior: " .. b.name .. " | Action Left: " .. action_left .. " | Action Right: " .. action_right)

            left_speed, right_speed = action_left, action_right
            return left_speed, right_speed
        end
    end

    log("[Controller] No behavior activated, using default speeds: Left: " .. left_speed .. " | Right: " .. right_speed)

    return left_speed, right_speed
end

return SubsumptionController
