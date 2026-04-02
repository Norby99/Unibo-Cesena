local utils = require("libraries.utils")
local Behavior = require("libraries.behaviors.behavior")

local RandomWander = setmetatable({}, {__index = Behavior})
RandomWander.__index = RandomWander

--[[ This behavior makes the robot wander randomly, changing direction every num_ticks ticks.
    max_velocity: the maximum velocity of the robot
    sensors: the sensors used by the behavior (not used in this behavior, but kept for
    consistency with the other behaviors and potential future use)
    num_ticks: the number of ticks after which the robot changes direction
    halt_behaviors: a list of behaviors that can halt this behavior (i.e., if
]]
function RandomWander.new(max_velocity, sensors, num_ticks, halt_behaviors)
    halt_behaviors = halt_behaviors or {}
    local self = setmetatable(Behavior.new("random_wander", max_velocity, sensors, halt_behaviors), RandomWander)

    self.num_ticks = num_ticks
    self.current_tick = num_ticks

    self.angle = 0

    return self
end

--[[ Performs the action of wandering randomly. The robot changes direction every num_ticks ticks.
    Returns:
        - A vector with length equal to max_velocity and angle equal to the current random direction
          if the behavior is activated (i.e., the robot should wander randomly)
        - nil if the behavior is not activated (i.e., the robot should not wander randomly)
]]
function RandomWander:action()
    if self:should_halt() then
        return nil
    end

    local max_value = 1

    if self.current_tick >= self.num_ticks then
        self.angle = math.random() * 2 * math.pi
        self.current_tick = 0
    end
    self.current_tick = self.current_tick + 1

    b_vec = {length = max_value, angle = self.angle}

    log("[RandomWander] intensity: " .. b_vec.length .. " | angle: " .. b_vec.angle)

    return b_vec
end

return RandomWander
