# ToDo

List of todo things in tiramisu project

## PRIORITARY todos, decreasing in priority order

- [ ] Modify the output method of the caffe branch, to write a binary file. Addapt the tiramisu input code to binary format.
- [ ] Read and store the statistics of 1,000 or so layer's features
- [ ] Test cosine distance similarities and differences
- [ ] Design an accumulative-proportional clustering algorithm that combines deviation strength and multi-layer similarities. Test.

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

## Todos related with general tiramisu research

- [ ] Adjust the threshold parameters to distribution of activations. Study which "mean+(stdDev*x)" fits a log-scale distribution.
- [ ] Test if additional edges between features are relevant and useful for the mining process. Consider adding layers as vertices, and features belonging to layers as edges.
- [ ] Research and implement community detection algorithms on the graph generated. Explore algorithms based on accumulative/proportional ratios (e.g., like INF_LOG_2D) 
- [ ] Is it interesting to shift from a map-vector based implementation to a vector-index or CRS implementation?
- [ ] Test euclidean distance on image vector representation
