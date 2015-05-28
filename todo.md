# ToDo

List of todo things in tiramisu project

## Todos related with scripts/bulk_feature_extraction.sh

- [ ] Modify bulk_feature_extraction.sh to make it path independent. Add a second parameter linking to caffe instalation
- [ ] Modify bulk_feature_extraction.sh to parametrize the layers being extracted. A third parameter separated by commas, or a file containing the layers.
- [ ] Modify bulk_feature_extraction.sh to check if all the required files exist before starting the process. These include the compiled caffe code, the net model and the CNN description file (prototxt)
- [ ] Modify bulk_feature_extraction.sh so that first parameter can be a relative path.

## Todos related with src/*

- [ ] Make sure include policy (relative path with "../include") is appropriate
