# ToDo

List of todo things in tiramisu project

## PRIORITARY todos, in priority order

- [x] Modify the output method of the caffe branch, to write a binary file. Addapt the tiramisu input code to binary format.
- [ ] Test cosine distance similarities and differences of the raw activations of a set of images X
- [ ] Compute the statistics of features by considering a set of images Y
- [ ] Build a vector of normalized relevant activations strengths e.g., mean+(X*stdDev)<value, if(x=1)val=0.25, if(x=2)val=0.5,...
- [ ] Test cosine distance similarities and differences of the relevant features of a set images disjoint with Y.
- [ ] Design an accumulative-proportional community detection algorithm that combines deviation strength and multi-layer similarities.
- [ ] Test community detection algorithm on a set of images disjoint with Y.

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
- [ ] If first parameter of main does not end with "/" it will crash. Fix it.

## Todos related with general tiramisu research

- [ ] Adjust the threshold parameters to distribution of activations. Study which "mean+(stdDev*x)" fits a log-scale distribution.
- [ ] Test if additional edges between features are relevant and useful for the mining process. Consider adding layers as vertices, and features belonging to layers as edges.
- [ ] Is it interesting to shift from a map-vector based implementation to a vector-index or CRS implementation for the activation vectors of images?
