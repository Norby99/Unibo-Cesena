local Behavior = {}
Behavior.__index = Behavior

-- Base constructor for a behavior
function Behavior.new(name, max_velocity, sensors)
    local self = setmetatable({}, Behavior)
    self.name = name
    self.max_velocity = max_velocity
    self.sensors = sensors
    return self
end

--[[ Performs the action of the behavior.
    This function should be overridden by subclasses.
    Returns: left_speed, right_speed (or nil if not activated)
]]
function Behavior:action()
    error("The method action() must be implemented by the subclass: " .. self.name)
end

return Behavior