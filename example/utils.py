import numpy as np

def get_and_add_box(dumbell_indices,boxes,data, i):
    l,r = dumbell_indices[i]
    # loop loop loop loop
    lbox = np.array([[min(x),max(x)] for x in data[l].T]).T
    rbox = np.array([[min(x),max(x)] for x in data[r].T]).T
    boxes.append([lbox,rbox])
    
def compute_centers(boxes, lcenters, rcenters, i):
    lc, rc = boxes[i]
    lcenters.append( (lc[0,:] + lc[1,:])/2)
    rcenters.append( (rc[0,:] + rc[1,:])/2)
 
