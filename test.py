

k = 6
numCoreSwitchs = int(k*k/4)
numPods = k
numSwitchPerPod = k
numServers = k*k*k/4

def agg(pod):
    result = list()
    for numAgg in range(int(numSwitchPerPod / 2)):
        result.append(int(numCoreSwitchs + int(numSwitchPerPod / 2)*pod + numAgg))
    return result

