#include "victim_localization/nbv_history.h"

nbv_history::nbv_history():
 iteration(0),
 max_prob(0),
 accumulated_utility(0),
 PathPlannerUsed(false)
{

}

void nbv_history::computeEntropyDiff()
{
  if (iteration==0)
  {
    entropy_diff.push_back(0);
  }
  else
  {
    double diff = total_entropy[iteration] - total_entropy[iteration-1];
    entropy_diff.push_back(diff);
  }
}

double nbv_history::getMaxUtility(int N_iterations)
{
  if (N_iterations > iteration)
    return std::numeric_limits<double>::quiet_NaN();


  // Get max entropy difference per voxel in the past "N_iterations"
  double max_utility = -std::numeric_limits<float>::infinity();
  for (int i=0; i<N_iterations; i++)
  {
    float utility = selected_utility[iteration-i];
    if (utility > max_utility)
      max_utility = utility;
  }

  return max_utility;
}


bool nbv_history::getMaxEntropyChange(int N_iterations, double threshold_)
{
  if (N_iterations > iteration)
    return std::numeric_limits<double>::quiet_NaN();


   std::vector<double> EntropyStatus;
   EntropyStatus.insert(EntropyStatus.end(),entropy_diff.begin(),entropy_diff.end());


  // Get max entropy difference per voxel in the past "N_iterations"
  double max_utility = -std::numeric_limits<float>::infinity();
  for (int i=0; i<N_iterations; i++)
  {
      std::cout << "last entropy value" << EntropyStatus.back()  << std::endl;
      std::cout << "entropy thresolod" << threshold_ << std::endl;

    if (fabs(EntropyStatus.back()) > threshold_)
      return true;
    EntropyStatus.pop_back();
  }

  return false;
}

bool nbv_history::isRepeatingMotions(int window_size)
{
  // Checks if the last 4 moves are repeated, indicating a local minima
  if (window_size >= iteration)
    return false;

  for (int i=1; i<=window_size-2; i++)
  {
    if (selected_utility[iteration-i] != selected_utility[iteration-i-2])
      return false;
  }

  return true;
}

void nbv_history::update()
{
  computeEntropyDiff();
  iteration++;
}
