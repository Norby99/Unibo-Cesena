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

    local n = #self.behaviors
    local left = 0
    local right = 0
    local total_weight = 0

    for i, b in ipairs(self.behaviors) do
        local l, r = b:action()
        local priority_weight = (n - i + 1) * b.weight

        left = left + l * priority_weight
        right = right + r * priority_weight
        total_weight = total_weight + priority_weight

        log("w: (" .. b.name .. ") = " .. priority_weight)
    end

    if total_weight > 0 then
        left = left / total_weight
        right = right / total_weight
    end

    return left, right
end

return Behavior
