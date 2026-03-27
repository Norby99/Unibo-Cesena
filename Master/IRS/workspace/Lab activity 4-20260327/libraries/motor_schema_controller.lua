local MotorSchemaController = {}
MotorSchemaController.__index = MotorSchemaController

local vector = require("libraries.vector")
local db = require("libraries.roboto_debug")

function MotorSchemaController.new(max_velocity)
    local self = setmetatable({}, MotorSchemaController)
    self.behaviors = {}
    self.max_velocity = max_velocity
    return self
end

-- Adds a behavior to the controller's list of behaviors.
function MotorSchemaController:add(behavior)
    table.insert(self.behaviors, behavior)
end

function MotorSchemaController:run()
    local n = #self.behaviors
    local left_speed = self.max_velocity
    local right_speed = self.max_velocity
    local total_weight = 0

    local sum_vector = {angle = 0, length = 0}

    for _, b in ipairs(self.behaviors) do
        local vec = b:action()
        sum_vector = vector.vec2_polar_sum(sum_vector, vec)
    end

    db.print_polar_vec("sum_vector", sum_vector)

    left_speed, right_speed = self:calculate_speed(sum_vector)

    return left_speed, right_speed
end

function MotorSchemaController:calculate_speed(v)
    local target_speed = self.max_velocity  -- = v.length * self.max_velocity
    
    local forward_component = target_speed * math.cos(v.angle)
    local turn_component = target_speed * math.sin(v.angle)
    
    left_speed = forward_component - turn_component
    right_speed = forward_component + turn_component
    return left_speed, right_speed
end

return MotorSchemaController
