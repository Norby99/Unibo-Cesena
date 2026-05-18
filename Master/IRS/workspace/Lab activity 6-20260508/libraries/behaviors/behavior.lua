local Behavior = {}
Behavior.__index = Behavior

--[[ Base constructor for a behavior
    name: string, the name of the behavior
    max_velocity: number, the maximum velocity of the robot
    sensors: table, the sensors of the robot
    halt_behaviors: table, a list of behaviors that can halt this behavior (optional)
]]
function Behavior.new(name, max_velocity, sensors, halt_behaviors)
    local self = setmetatable({}, Behavior)
    self.name = name
    self.max_velocity = max_velocity
    self.sensors = sensors
    self.halt_behaviors = halt_behaviors or {}
    return self
end

--[[ Checks if any of the halt behaviors is activated.
    Returns true if at least one halt behavior is activated, false otherwise.
]]
function Behavior:should_halt()
    for _, halt_behavior in ipairs(self.halt_behaviors) do
        if halt_behavior:action() ~= nil then
            return true
        end
    end
    return false
end

--[[ Performs the action of the behavior.
    This function should be overridden by subclasses.
    Returns: left_speed, right_speed (or nil if not activated)
]]
function Behavior:action()
    error("The method action() must be implemented by the subclass: " .. self.name)
end

return Behavior