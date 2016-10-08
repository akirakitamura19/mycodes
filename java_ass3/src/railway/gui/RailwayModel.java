package railway.gui;

import java.io.IOException;
import java.util.*;
import railway.FormatException;
import railway.Route;
import railway.RouteReader;
import railway.TrackReader;
import railway.Track;

/**
 * The model for the Railway Manager.
 */
public class RailwayModel {

    // the track that will be loaded
    private Track track;
    // the route that will be loaded
    private Route route;
    // the sub-route that will be loaded
    private Route subRoute;
    // A List of previously allocated sub-routes. None of these sub-routes
    // intersects.
    private ArrayList<Route> subRouteList;
    // A List of previously allocated routes.
    private ArrayList<Route> routeList;
    // A List of previously allocated trains' startOffset.
    private ArrayList<Object> startOffList;
    // A List of previously allocated trains' startOffset.
    private ArrayList<Object> endOffList;
    // the integer for an startOffset
    private int start;
    // the integer for an endOffset
    private int end;

    /**
     * Initialises the model for the Railway Manager.
     */
    public RailwayModel() {
        track = new Track();
        routeList = new ArrayList<Route>();
        subRouteList = new ArrayList<Route>();
        startOffList = new ArrayList<>();
        endOffList = new ArrayList<>();
    }

    /**
     * Load the track file
     * 
     * @param filename
     *            file name of a track
     */
    public void loadTrack(String file) throws IOException, FormatException {
        track = new Track();
        track = TrackReader.read(file);
    }

    /**
     * Load the route file
     * 
     * @param filename
     *            file name of a route
     */
    public void loadRoute(String file)
            throws IOException, FormatException, NullPointerException {
        route = RouteReader.read(file);
        routeList.add(route);
    }

    /**
     * Check whether the route file on the track or not.
     * 
     * @return true or false whether the route on the track
     */
    public boolean checkRouteOnTrack() throws NullPointerException {
        return route.onTrack(track);
    }

    /**
     * Check whether the startOffset and endOffset are valid as a subRoute or
     * not. And stock the start and end-offset.
     * 
     * @param startOffset
     *            and endOffset
     */
    public void checkValidStartAndEndOffset(int startOffset, int endOffset)
            throws IllegalArgumentException {
        subRoute = route.getSubroute(startOffset, endOffset);
        start = startOffset;
        end = endOffset;
    }

    /**
     * Check whether the sub-route is not intersects with other sub-routes.
     * 
     * @return true or false whether or not the sub-route is intersected with
     *         other sub-routes.
     */
    public boolean checkIntersects1() {
        boolean checker;
        if (subRouteList.size() >= 1) {
            for (int i = 1; i == subRouteList.size(); i++) {
                checker = subRoute.intersects(subRouteList.get(i - 1));
                if (checker == true) {
                    return true;
                }
            }
        }
        subRouteList.add(subRoute);
        startOffList.add(start);
        endOffList.add(end);
        return false;
    }

    /**
     * Check whether the sub-route is not intersects with other sub-routes for
     * updating.
     * 
     * @param Identifiler
     *            which is selected.
     * 
     * @return true or false whether or not the sub-route is intersected with
     *         other sub-routes.
     */
    public boolean checkIntersects2(int identifiler) {
        boolean checker;
        if (subRouteList.size() >= 1) {
            for (int i = 1; i == subRouteList.size(); i++) {
                if (i - 1 == identifiler) {
                    continue;
                }
                checker = subRoute.intersects(subRouteList.get(i - 1));
                if (checker == true) {
                    return true;
                }
            }
        }
        subRouteList.set(identifiler, subRoute);
        startOffList.set(identifiler, start);
        endOffList.set(identifiler, end);
        return false;
    }

    /**
     * Check whether the identifier is valid or not.
     * 
     * @param Identifiler
     *            which is selected.
     * 
     * @return true or false. whether or not the sub-route is intersected with
     *         other sub-routes.
     */
    public boolean checkIdentifier(int identifier) {
        if (identifier <= subRouteList.size() - 1) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * Return the result of the Sub-route
     * 
     * @return Information of the identifier, startOffset, endOffset and the
     *         route.
     */
    public String viewResult(int identifier) {
        String info = "Identifier: \n" + String.valueOf(identifier)
                + "\n\nStartOffset: \n"
                + String.valueOf(startOffList.get(identifier))
                + "\n\nEndOffset: \n"
                + String.valueOf(endOffList.get(identifier))
                + "\n\nRoute information: \n"
                + String.valueOf(routeList.get(identifier));
        return info;
    }

}
