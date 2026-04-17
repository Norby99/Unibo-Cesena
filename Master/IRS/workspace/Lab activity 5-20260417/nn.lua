local nn = {}

local function sigmoid(x)
    return 1 / (1 + math.exp(-x))
end

local function randomWeight()
    return (math.random() * 2 - 1) -- range [-1, 1]
end


-- Create a neural network
function nn.createNetwork(inputSize,outputSize,weight)
-- size of weights = inputSize * outputSize + outputSize
    local net = {}

    net.inputSize = inputSize
    net.outputSize = outputSize

    -- weights is a vector of fp numbers, read sequentially to initialize the network weights
    -- each block of size outputSize represents the weights from an input node to the output nodes
    net.weight = {}
    local k = 0
    for i = 1, inputSize do
        net.weight[i] = {}
        for j = 1, outputSize do
            k = k + 1
            net.weight[i][j] = weight[k]
        end
    end

    -- Biases of output nodes are the last elements of weights
    net.bias = {}
    for j = 1, outputSize do
        net.bias[j] = weight[inputSize*outputSize + j]
    end

    return net
end


-- Computation of network output values
function nn.forward(network,input)
    local output = {}
    for j = 1, network.outputSize do
        local sum = network.bias[j]
        for i = 1, network.inputSize do
            sum = sum + input[i] * network.weight[i][j]
        end
        output[j] = sigmoid(sum)
    end
    return output
end

return nn
