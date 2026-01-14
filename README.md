```
             ____   __                                     _ _      
   __ _  ___|___ \ / _| _ ___  __ _ ___ _   _ _ __ ___  __| (_)_ __ 
  / _` |/ _ \ __) | |(_|_) _ \/ _` / __| | | | '__/ _ \/ _` | | '__|
 | (_| |  __// __/|  _| |  __/ (_| \__ \ |_| | | |  __/ (_| | | |   
  \__,_|\___|_____|_|(_|_)___|\__,_|___/\__, |_|  \___|\__,_|_|_|   
                                        |___/
```

> A cross-platform utility for easy pipelining.  
> Creates sub process with pipeline

# Language
- strict ISO C89

# Build
Normal cmake building will be enough.

```sh
cmake -Bbuild
cmake --build build
```

# Usage
```sh
easypipe stdin_file stdout_file stderr_file is_append programme-to-execute __VA_ARGS__|null
# is_append must be integer.
```
