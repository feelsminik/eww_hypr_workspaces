C application to connect to Hyprland IPC, filtering and formatting it's output in `json` format for creating desktop widgets using `eww`. 
Predecessor of [minikbar](https://www.github.com/feelsminik/minikbar)

Dependencies:
- [EWW wacky widgets](https://github.com/elkowar/eww)
- [cJSON](https://github.com/DaveGamble/cJSON)
- Otherwise its a C based project

❌ Reason why it's not in use anymore:
- EWW's config language is based on a lisp dialect which makes it unintuitive to me
- More complex EWW widgets rely on custom written scripts/binaries, which actually means I need to write most of the backend part myself anyway
