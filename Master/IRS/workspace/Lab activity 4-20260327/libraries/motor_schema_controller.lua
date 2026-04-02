local MotorSchemaController = {}
MotorSchemaController.__index = MotorSchemaController

local vector = require("libraries.vector")
local utils = require("libraries.utils")
local db = require("libraries.roboto_debug")

function MotorSchemaController.new(max_velocity, weights)
    local self = setmetatable({}, MotorSchemaController)
    self.behaviors = {}
    self.max_velocity = max_velocity
    self.weights = weights or {}
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

    local sum_vector = {length = 0, angle = 0}

    for _, b in ipairs(self.behaviors) do
        local vec = b:action()

        if vec then
            local weight = self.weights[b.name] or DEFAULT_WEIGHT
            log("[Controller] behavior: " .. b.name .. " | weight: " .. weight)
            vec.length = vec.length * weight
        end

        sum_vector = vector.vec2_polar_sum(sum_vector, vec)
    end

    left_speed, right_speed = self:calculate_speed(sum_vector)

    db.print_polar_vec("sum_vector", sum_vector)
    log("[Controller] left_speed: " .. left_speed .. " | right_speed: " .. right_speed)

    return left_speed, right_speed
end

function MotorSchemaController:calculate_speed(vec)
    local L = robot.wheels.axis_length

    v = self.max_velocity * vec.length
    w = vec.angle

    left_speed = v - (L / 2) * w
    right_speed = v + (L / 2) * w

    return utils.normalize_speeds(left_speed, right_speed, self.max_velocity)
end

return MotorSchemaController
