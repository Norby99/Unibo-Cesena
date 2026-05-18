local MotorSchemaController = {}
MotorSchemaController.__index = MotorSchemaController

local vector = require("libraries.vector")
local utils = require("libraries.utils")
local db = require("libraries.roboto_debug")

-- Manages the motor schemas, combining each behavior's vector output.
function MotorSchemaController.new(max_velocity, weights)
    local self = setmetatable({}, MotorSchemaController)
    self.behaviors = {}
    self.max_velocity = max_velocity
    self.weights = weights or {}

    self.MOVING = true;
    self.PROB = {Ps = 0.5, Pw = 0.5}

    return self
end

-- Adds a behavior to the controller's list of behaviors.
function MotorSchemaController:add(behavior)
    table.insert(self.behaviors, behavior)
end

--[[ Runs the controller, calculating the combined vector from all behaviors and converting it to wheel speeds.
     Returns:
     - left_speed: The speed for the left wheel.
     - right_speed: The speed for the right wheel.
]]
function MotorSchemaController:run()
    local n = #self.behaviors
    local left_speed = self.max_velocity
    local right_speed = self.max_velocity

    local sum_vector = {length = 0, angle = 0}

    self:calculate_prob()

    if self.MOVING then
        robot.leds.set_all_colors("green")
        robot.range_and_bearing.set_data(1,0)
    else
        robot.leds.set_all_colors("red")
        robot.range_and_bearing.set_data(1,1)
    end

    local rn = robot.random.uniform()
    if self.MOVING then
        if rn <= self.PROB.Ps then
            self.MOVING =  false
        end
    else
        if rn <= self.PROB.Pw then
            self.MOVING =  true
        end

        return 0, 0
    end

    for _, b in ipairs(self.behaviors) do
        local vec = b:action()

        if vec then
            local weight = self.weights[b.name] or DEFAULT_WEIGHT
            -- log("[Controller] behavior: " .. b.name .. " | weight: " .. weight)
            vec.length = vec.length * weight
        end

        sum_vector = vector.vec2_polar_sum(sum_vector, vec)
    end

    left_speed, right_speed = self:calculate_speed(sum_vector)

    --db.print_polar_vec("sum_vector", sum_vector)

    return left_speed, right_speed
end

function MotorSchemaController:calculate_prob()
    CountRAB()

    -- for the first exercise set PARAMS.D to 0
    self.PROB.Ps = math.min(PARAMS.Psmax, PARAMS.S + PARAMS.a * PARAMS.N + PARAMS.D)
    self.PROB.Pw = math.max(PARAMS.Pwmin, PARAMS.W - PARAMS.b * PARAMS.N + PARAMS.D)

    --log("[Controller] N: " .. PARAMS.N .. " | Ps: " .. self.PROB.Ps .. " | Pw: " .. self.PROB.Pw)
end

function CountRAB()
    number_robot_sensed = 0
    for i = 1, #robot.range_and_bearing do
        -- for each robot seen, check if it is close enough.
        if robot.range_and_bearing[i].range < PARAMS.MAXRANGE and robot.range_and_bearing[i].data[1]==1 then
            number_robot_sensed = number_robot_sensed + 1
        end
    end
    PARAMS.N = number_robot_sensed
end

-- Converts a polar vector to left and right wheel speeds using a differential drive model.
function MotorSchemaController:calculate_speed(vec)
    local L = robot.wheels.axis_length

    v = self.max_velocity * vec.length
    w = vec.angle

    left_speed = v - (L / 2) * w
    right_speed = v + (L / 2) * w

    return utils.normalize_speeds(left_speed, right_speed, self.max_velocity)
end

return MotorSchemaController
