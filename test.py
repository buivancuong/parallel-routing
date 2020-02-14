

k = 64
numCoreSwitchs = int(k*k/4)
numPods = k
numSwitchPerPod = k
numServers = k*k*k/4

def edge(pod):
    result = list()
    for numEdge in range(int(numSwitchPerPod / 2)):
        result.append(int(numCoreSwitchs + int(numSwitchPerPod / 2)*numPods + int(numSwitchPerPod / 2)*pod + numEdge))
    return result

def server(edge):
    result = list()
    for numServer in range(int(numSwitchPerPod / 2)):
        result.append(int(numCoreSwitchs + numSwitchPerPod*numPods + int(numSwitchPerPod / 2)*edge + numServer))
    return result

for pod in range(numPods):
    for numEdge in range(int(numSwitchPerPod / 2)):
        edge = numCoreSwitchs + int(numSwitchPerPod / 2)*numPods + int(numSwitchPerPod / 2)*pod + numEdge
        for numServer in range(int(k / 2)):
            server = numCoreSwitchs + numSwitchPerPod*numPods + int(numSwitchPerPod / 2)*numEdge + int(numSwitchPerPod / 2)*int(numSwitchPerPod / 2)*pod + numServer
            print(str(edge) + " --> " + str(server))
        print("\n")
    print("***********")