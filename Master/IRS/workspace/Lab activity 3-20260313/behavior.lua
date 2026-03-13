local Behavior = {}
Behavior.__index = Behavior

function Behavior.new()
    local self = setmetatable({}, Behavior)
    self.behaviors = {}
    return self
end

function Behavior:add(behavior)
    table.insert(self.behaviors, behavior)
end

function Behavior:run()

    local left = 0
    local right = 0

    for _, b in ipairs(self.behaviors) do
        local l, r = b:action()

        left = left + l * b.weight
        right = right + r * b.weight
    end

    return left, right
end

return Behavior
