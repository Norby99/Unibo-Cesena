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

    log("[Controller] left_speed: " .. left_speed .. " | right_speed: " .. right_speed)

    return left_speed, right_speed
end

function MotorSchemaController:calculate_speed(v)
    local target_speed = self.max_velocity
    
    local left_speed = target_speed
    local right_speed = target_speed
    
    local angle = v.angle
    
    if angle > 0 then
        left_speed = target_speed * (1 - (angle / math.pi))
    else
        right_speed = target_speed * (1 - (-angle / math.pi))
    end

    return left_speed, right_speed
end

return MotorSchemaController
