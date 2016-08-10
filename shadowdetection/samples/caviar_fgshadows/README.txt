Shadow Ground Truth Data for CAVIAR

(C) 2009 Andres Sanin
andres.sanin@nicta.com.au

This dataset contains shadow ground truth data for 25 sequences
from the second set (corridor view) of the CAVIAR dataset.
CAVIAR can be found at http://homepages.inf.ed.ac.uk/rbf/CAVIARDATA1/

Each folder in the dataset contains ground truth data for one CAVIAR sequence,
and each has the following contents:
  - frames: original sampled frames
  - bgs: background estimation using a Gaussian mixture model approach
  - shadows: final masks with three possible values:
             0 for background pixels,
             255 for foreground pixels, 
             127 for shadow pixels.


The ground truth was generated as follows:

1.
A sample of frames was selected for each sequence.

2.
The foreground mask was obtained for each sampled frame using
the Gaussian mixture model background subtraction implemented
in OpenCV v2.0 with the following parameters:
  - n gaussians: 5
  - window size: 200
  - bg threshold: 0.7
  - std threshold: 2.5
  - initial weight: 0.05
  - initial variance: 900
  - minimum area: 15

3.
Pixels from the foreground mask were hand labelled as either foreground objects
or cast shadows.  Frames that produced empty masks were discarded.


