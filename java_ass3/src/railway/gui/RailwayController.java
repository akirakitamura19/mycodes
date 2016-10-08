package railway.gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import railway.FormatException;

/**
 * The controller for the Railway Manager.
 */
public class RailwayController {

    // The model that is being controlled.
    private RailwayModel model;
    // The view that is being controlled.
    private RailwayView view;
    // The status for checking the current situation.
    private int status = 0;

    /**
     * Initialises the Controller for the Railway Manager.
     */
    public RailwayController(RailwayModel model, RailwayView view) {
        this.model = model;
        this.view = view;
        tryLoadTrackFile();
        view.addTrainListener(new AddTrainActionListener());
        view.updateTrainListener(new UpdateTrainActionListener());
        view.viewTrainListener(new ViewTrainActionListener());
    }

    /**
     * Load the track file and show up meaning message. Otherwise, show up
     * meaning error message.
     */
    public void tryLoadTrackFile() {
        try {
            model.loadTrack("track.txt");
            view.showInfoMessage("Loading track file has succeeded.");
        } catch (IOException e0) {
            view.showErrorMessage(e0.getMessage());
        } catch (FormatException e0) {
            view.showErrorMessage(e0.getMessage());
        }
    }

    /**
     * Load the route file and set up start & end-offset. Otherwise, show up
     * meaning error message.
     */
    private class AddTrainActionListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            try {
                // Route file name that was inputed
                String filename = view.inputRoute();
                model.loadRoute(filename);
                checkRoute();
                if (status == 1) {
                    try {
                        // StartOffset and EndOffset that were inputed.
                        int startOffset = view.inputStartOffset();
                        int endOffset = view.inputEndOffset();
                        checkSubRoute1(startOffset, endOffset);
                        if (status == 2) {
                            checkSubRoute2();
                            if (status == 3) {
                                view.showInfoMessage(
                                        "New sub-route has set up.");
                            }
                        }
                    } catch (NumberFormatException e1) {
                        view.showErrorMessage("You must input an integer.");
                    }
                }
            } catch (IOException e1) {
                view.showErrorMessage(e1.getMessage());

            } catch (FormatException e1) {
                view.showErrorMessage(e1.getMessage());

            } catch (NullPointerException e1) {
                view.showErrorMessage("No file name is inputed.");
            }
        }
    }

    /**
     * Check the route on the track. If not, show up meaningful error message.
     */
    private void checkRoute() {
        try {
            // Boolean for check whether the route is on the track.
            boolean checker = model.checkRouteOnTrack();
            if (checker == false) {
                view.showErrorMessage("The route is not on the track");
            } else {
                status = 1;
            }
        } catch (NullPointerException e2) {
            view.showErrorMessage(e2.getMessage());
        }
    }

    /**
     * Check whether the the startOffset and endOffset are valid as a subRoute.
     * If not, show up meaningful error message.
     * 
     * @param startOffset and endOffset
     */
    private void checkSubRoute1(int startOffset, int endOffset) {
        try {
            model.checkValidStartAndEndOffset(startOffset, endOffset);
            status = 2;
        } catch (IllegalArgumentException e3) {
            view.showErrorMessage(e3.getMessage());
        }
    }

    /**
     * Check whether the subRoute is intersected with other sub-routes. If so,
     * show up meaningful error message.
     */
    private void checkSubRoute2() {
        // Checker for checking whether the sub-route is not intersect with
        // other sub-routes.
        boolean checker = model.checkIntersects1();
        if (checker == true) {
            view.showErrorMessage("This subroute is intersected with other.");
        } else {
            status = 3;
        }
    }

    /**
     * Update the startOffset and endOffset of sub-route. Otherwise, show up
     * meaning error message.
     */
    private class UpdateTrainActionListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            try {
                // Identifier of selected train, which is an integer.
                int identifier = view
                        .inputIdentifier("Which train do you want to update?");
                // Checker to check whether or not inputed identifier is
                // existed.
                boolean checker = model.checkIdentifier(identifier);
                if (checker) {
                    try {
                        // New start & endOffset which user want to move.
                        int newStartOffset = view.inputStartOffset();
                        int newEndOffset = view.inputEndOffset();
                        checkSubRoute1(newStartOffset, newEndOffset);
                        if (status == 2) {
                            checkSubRoute3(identifier);
                            if (status == 3) {
                                view.showInfoMessage(
                                        "New sub-route has set up.");
                            }
                        }
                    } catch (NumberFormatException e4) {
                        view.showErrorMessage("You must input an integer.");
                    }

                } else {
                    view.showErrorMessage("The selected train does not exist.");
                }
            } catch (NumberFormatException e4) {
                view.showErrorMessage("Identifier must be an integer.");
            }
        }
    }

    /**
     * View the result of a particular selected train Otherwise, show up
     * meaningful error message.
     */
    private class ViewTrainActionListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            try {
                // Identifier of a train which user want to view the result
                int identifier = view
                        .inputIdentifier("Which train do you want to look at?");
                boolean checker = model.checkIdentifier(identifier);
                if (checker) {
                    view.showInfoMessage(model.viewResult(identifier));
                } else {
                    view.showErrorMessage("The selected train does not exist.");
                }

            } catch (NumberFormatException e5) {
                view.showErrorMessage("Identifier must be an integer.");
            }
        }
    }

    /**
     * Check whether the subRoute is intersected with other sub-routes for
     * updating . If so, show up meaningful message.
     * 
     * @param Identifier
     *              which is selected by user.
     */
    private void checkSubRoute3(int identifier) {
        // Checker to check whether or not the sub-route of the identifier is
        // intersected with other sub-routes.
        boolean checker = model.checkIntersects2(identifier);
        if (checker == true) {
            view.showErrorMessage("This subroute is intersected with other.");
        } else {
            status = 3;
        }
    }
}
