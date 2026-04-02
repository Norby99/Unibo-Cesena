local WeightsManager = {}
WeightsManager.__index = WeightsManager

function WeightsManager.new(weights)
    local self = setmetatable({}, WeightsManager)
    self.weights = weights or {}

    self.WEIGHT_SUM_GOAL = 1.0
    self.WEIGHT_MARGIN = 0.001

    return self
end

function WeightsManager:get_weights()
    local sum = 0
    for _, weight in pairs(self.weights) do
        sum = sum + weight
    end
    
    assert(math.abs(sum - self.WEIGHT_SUM_GOAL) < self.WEIGHT_MARGIN,
        "The sum of the weights must be equal to "
        .. tostring(self.WEIGHT_SUM_GOAL)
        .. ". Current sum: "
        .. tostring(sum))
    
    return self.weights
end

return WeightsManager
