```
             _____  __                              _            
   __ _  ___|___ / / _| _ ___  __ _ ___ _   _ _ __ (_)_ __   ___ 
  / _` |/ _ \ |_ \| |(_|_) _ \/ _` / __| | | | '_ \| | '_ \ / _ \
 | (_| |  __/___) |  _| |  __/ (_| \__ \ |_| | |_) | | |_) |  __/
  \__,_|\___|____/|_|(_|_)___|\__,_|___/\__, | .__/|_| .__/ \___|
                                        |___/|_|     |_|
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
```
easypipe stdin_file stdout_file stderr_file programme-to-execute __VA_ARGS__|null
```
