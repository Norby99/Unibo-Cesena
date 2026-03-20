local SubsumptionController = {}
SubsumptionController.__index = SubsumptionController

function SubsumptionController.new(max_velocity)
    local self = setmetatable({}, SubsumptionController)
    self.behaviors = {}
    self.max_velocity = max_velocity
    return self
end

function SubsumptionController:add(behavior)
    table.insert(self.behaviors, behavior)
end

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
