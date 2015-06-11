# ToDo

List of todo things in tiramisu project

## Todos related with scripts/

- [ ] Modify bulk_feature_extraction.sh to make it path independent. Add a second parameter which indicates location of caffe instalation
- [ ] Modify bulk_feature_extraction.sh to parametrize the layers being extracted, reading them from a file.
- [ ] Modify bulk_feature_extraction.sh to check if all the requirements exist before starting the process. These include the compiled caffe code, the net model and the CNN description file (prototxt).
- [ ] Modify bulk_feature_extraction.sh so that first parameter can be a relative path.
- [ ] Figure out how to use batches of images in the extraction process. Handle output files.

## Todos related with src/

- [ ] Make sure include policy (relative path with "../include") is appropriate.
- [ ] Write an interactive menu for loading layers and images from files.
- [ ] Parallelize with OpenMP.
