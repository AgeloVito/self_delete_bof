# self_delete_bof
BOF implementation of delete self poc that delete a locked executable or a currently running file from disk by its pid, path, or the current process.

## Build

```sh
git clone https://github.com/AgeloVito/self_delete_bof.git
```

```sh 
make
```

**And load self_delete.cna**


## Usage
```
beacon> help self_delete
Use: self_delete [pid|path]
e.g: self_delete
     self_delete 7956
     self_delete /path/file.exe
```
self_delete

<img width="1122" alt="image" src="https://github.com/AgeloVito/self_delete_bof/assets/9564171/7507a649-a074-4e15-a439-69dc5f5dca22">

self_delete by path

<img width="933" alt="image" src="https://github.com/AgeloVito/self_delete_bof/assets/9564171/e3703f74-2a5d-46b0-81f8-39c2e9a25442">

self_delete by pid

<img width="956" alt="image" src="https://github.com/AgeloVito/self_delete_bof/assets/9564171/8731b2ef-a6e6-48b3-bd78-e735d87b7ca7">

## Issues

x86 process without manifest

<img width="837" alt="image" src="https://github.com/AgeloVito/self_delete_bof/assets/9564171/32aedc7d-8688-4efa-8beb-2c1e892dc134">


## Refences

https://stackoverflow.com/questions/71834608/

https://github.com/LloydLabs/delete-self-poc

https://github.com/LloydLabs/delete-self-poc/pull/4/files
