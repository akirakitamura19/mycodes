package railway;

import java.util.*;

public class Allocator {

    /**
     * This method takes as input a list of the routes that are currently
     * occupied by trains on the track, and a list of the routes requested by
     * each of those trains and returns an allocation of routes to trains based
     * on those inputs.
     * 
     * Such a method may be used by a train controller to manage the movement of
     * trains on the track so that they do not collide. (I.e. if a train has
     * been allocated to a route, it has permission to travel on that route.)
     * 
     * @require occupied != null && requested != null
     * 
     *          && !occupied.contains(null)
     * 
     *          && !requested.contains(null)
     * 
     *          && occupied.size() == requested.size()
     * 
     *          && none of the occupied routes intersect
     * 
     *          && the routes in the occupied list are non-empty, valid routes
     * 
     *          && the routes in the requested list are non-empty, valid routes
     * 
     *          && all the routes in occupied and requested are part of the same
     *          track.
     * 
     * @ensure Let N be the number of elements in the occupied list. This method
     *         returns a list of N routes, where, for each index i satisfying 0
     *         <= i < N, \result.get(i) is the route allocated to the ith train:
     *         the train currently occupying route occupied.get(i).
     * 
     *         The route allocated to the ith train is the longest prefix of
     *         requested.get(i) that does not intersect with any of the routes
     *         currently occupied by any other train, or any of the routes
     *         \result.get(j) for indices j satisfying 0 <= j < i. (I.e. trains
     *         with lower indices have higher priority.)
     * 
     *         Neither of the two input parameters, the occupied list and the
     *         requested list, are modified in any way by this method.
     *
     * @param occupied
     *            there are occupied.size() trains on the track, and parameter
     *            occupied is a list of the routes currently occupied by each of
     *            those trains. A precondition of this method is that none of
     *            the occupied routes are null or empty, they are valid routes,
     *            and that they do not intersect (i.e. no two trains can occupy
     *            the same location on the track at the same time).
     * @param requested
     *            a list of the routes requested by each of the occupied.size()
     *            trains. A precondition of the method is that occupied.size()
     *            == requested.size(), and that none of the requested routes are
     *            null or empty, and that they are valid routes. For index i
     *            satisfying 0 <= i < requested.size(), requested.get(i) is the
     *            route requested by the train currently occupying the route
     *            occupied.get(i).
     * @return the list of allocated routes.
     */
    public static List<List<Segment>> allocate(List<List<Segment>> occupied,
            List<List<Segment>> requested) {

        // Create a new deep copy of requested
        List<List<Segment>> deepCopyRequested = new ArrayList<List<Segment>>();
        for (List<Segment> p : requested) {
            deepCopyRequested.add(new ArrayList<Segment>(p));
        }

        // Compare the each train's routes of deepCopyRequested with the routes
        // of occupied.
        deepCopyRequested = CompareWithOccupied(occupied, deepCopyRequested);

        // Compare the each train's routes of deepCopyRequested with the routes
        // of requested.
        deepCopyRequested = CompareWithRequested(requested, deepCopyRequested);

        return deepCopyRequested;
    }

    /**
     * Compare the each train's routes of requested with the routes of occupied.
     * There are 4 cases to modify deepCopyRequested.
     * 
     * case 1:
     * @require The train number of requested and occupied is same
     * @ensure No need to consider them and skip it.
     * 
     * case 2:
     * @require The route is completely overlapped with a route of occupied.
     * @ensure Remove the route.
     * 
     * case 3:
     * @require Not case1 && Not case2 && the StartOffset of the route in
     *          requested is contained in the route of occupied.
     * @ensure modify StartOffset of the route of deepCopyRequested not to
     *         overlap the routes of occupied.
     * 
     * case 4:
     * @require Not case1 && Not case2 && Not case3 && the EndOffset of the
     *          route in requested is contained in the route of occupied.
     * @ensure modify EndOffset of the route of deepCopyRequested not to overlap
     *         the routes of occupied.
     * 
     * 
     */
    private static List<List<Segment>> CompareWithOccupied(
            List<List<Segment>> occupied,
            List<List<Segment>> deepCopyRequested) {
        for (int a = 0; a < deepCopyRequested.size(); a++) {
            for (int b = 0; b < occupied.size(); b++) {
                for (int c = 0; c < deepCopyRequested.get(a).size(); c++) {
                    for (int d = 0; d < occupied.get(b).size(); d++) {

                        // case1
                        if (a == b) {
                            continue;
                        }

                        // case 2
                        else if (occupied.get(b).get(d)
                                .contains(deepCopyRequested.get(a).get(c)
                                        .getFirstLocation())
                                && occupied.get(b).get(d)
                                        .contains(deepCopyRequested.get(a)
                                                .get(c).getLastLocation())) {
                            deepCopyRequested.get(a).remove(c);
                            d = occupied.get(b).size();
                        }

                        // case 3
                        else if (occupied.get(b).get(d)
                                .contains(deepCopyRequested.get(a).get(c)
                                        .getFirstLocation())) {
                            deepCopyRequested = OccupiedContainFirstLocation(a,
                                    b, c, d, occupied, deepCopyRequested);
                        }

                        // case 4
                        else if (occupied.get(b).get(d)
                                .contains(deepCopyRequested.get(a).get(c)
                                        .getLastLocation())) {
                            deepCopyRequested = OccupiedContainLastLocation(a,
                                    b, c, d, occupied, deepCopyRequested);

                        }
                    }
                }
            }
        }
        return deepCopyRequested;
    }

    /**
     * case 3: There are 3 cases to consider in case3
     * 
     * case 3-1:
     * @require StartOffset is atJunction
     * @ensure StartOffset plus 1
     * 
     * case 3-2:
     * @require Not case 3-1 && directions of approchingPoint are same
     * @ensure StartOffset = EndOffset of occupied + 1
     * 
     * case 3-3:
     * @require Not case 3-1 && Not case 3-2 && directions of approchingPoint
     *          are different
     * @ensure StartOffset = Length - StartOffset of occupied + 1
     * 
     * 
     */
    private static List<List<Segment>> OccupiedContainFirstLocation(int a,
            int b, int c, int d, List<List<Segment>> occupied,
            List<List<Segment>> deepCopyRequested) {
        // case 3-1
        if (deepCopyRequested.get(a).get(c).getStartOffset() == 0) {
            deepCopyRequested.get(a).set(c, new Segment(
                    deepCopyRequested.get(a).get(c).getSection(),
                    deepCopyRequested.get(a).get(c).getDepartingEndPoint(),
                    deepCopyRequested.get(a).get(c).getStartOffset() + 1,
                    deepCopyRequested.get(a).get(c).getEndOffset()));
        }
        // case 3-2
        else if (occupied.get(b).get(d)
                .getApproachingEndPoint() == deepCopyRequested.get(a).get(c)
                        .getApproachingEndPoint()) {
            deepCopyRequested.get(a).set(c,
                    new Segment(deepCopyRequested.get(a).get(c).getSection(),
                            deepCopyRequested.get(a).get(c)
                                    .getDepartingEndPoint(),
                            occupied.get(b).get(d).getEndOffset() + 1,
                            deepCopyRequested.get(a).get(c).getEndOffset()));
        }
        // case 3-3
        else {
            deepCopyRequested.get(a).set(c, new Segment(
                    deepCopyRequested.get(a).get(c).getSection(),
                    deepCopyRequested.get(a).get(c).getDepartingEndPoint(),
                    occupied.get(b).get(d).getSection().getLength()
                            - occupied.get(b).get(d).getStartOffset() + 1,
                    deepCopyRequested.get(a).get(c).getEndOffset()));
        }
        return deepCopyRequested;
    }

    /**
     * case 4: There are 3 cases to consider in case4
     * 
     * case 4-1:
     * @require EndOffset is atJunction
     * @ensure EndOffset = EndOffset - 1
     * 
     * case 4-2:
     * @require Not case 4-1 && directions of approchingPoint are same
     * @ensure EndOffset = StartOffset of occupied - 1
     * 
     * case 4-3:
     * @require Not case 4-1 && Not case 4-2 && directions of approchingPoint
     *          are different
     * @ensure EndOffset = Length - EndOffset of occupied - 1
     * 
     */
    private static List<List<Segment>> OccupiedContainLastLocation(int a, int b,
            int c, int d, List<List<Segment>> occupied,
            List<List<Segment>> deepCopyRequested) {
        // case 4-1
        if (deepCopyRequested.get(a).get(c).getEndOffset() == deepCopyRequested
                .get(a).get(c).getSection().getLength()) {
            deepCopyRequested.get(a).set(c, new Segment(
                    deepCopyRequested.get(a).get(c).getSection(),
                    deepCopyRequested.get(a).get(c).getDepartingEndPoint(),
                    deepCopyRequested.get(a).get(c).getStartOffset(),
                    deepCopyRequested.get(a).get(c).getEndOffset() - 1));

        }
        // case 4-2
        else if (occupied.get(b).get(d)
                .getApproachingEndPoint() == deepCopyRequested.get(a).get(c)
                        .getApproachingEndPoint()) {
            deepCopyRequested.get(a).set(c,
                    new Segment(deepCopyRequested.get(a).get(c).getSection(),
                            deepCopyRequested.get(a).get(c)
                                    .getDepartingEndPoint(),
                            deepCopyRequested.get(a).get(c).getStartOffset(),
                            occupied.get(b).get(d).getStartOffset() - 1));

        }
        // case 4-3
        else {
            deepCopyRequested.get(a).set(c,
                    new Segment(deepCopyRequested.get(a).get(c).getSection(),
                            deepCopyRequested.get(a).get(c)
                                    .getDepartingEndPoint(),
                            deepCopyRequested.get(a).get(c).getStartOffset(),
                            occupied.get(b).get(d).getSection().getLength()
                                    - occupied.get(b).get(d).getEndOffset()
                                    - 1));
        }
        return deepCopyRequested;
    }

    /**
     * Compare the each train's routes of deepCopyRequested with the routes of
     * requested. There are 3 cases to modify deepCopyRequested.
     * 
     * case 1:
     * @require The train number of deepCopyRequested and requested is same or
     *          requested is bigger.
     * @ensure No need to consider them and skip it.
     * 
     * case 2:
     * @require The route is completely overlapped with a route of requested.
     * @ensure Remove the route.
     * 
     * case 3:
     * @require Not case1 && Not case2 && the StartOffset of the route in
     *          deepCopyRequested is contained in the route of requested.
     * @ensure modify StartOffset of the route of deepCopyRequested not to
     *         overlap the routes of requested.
     * 
     * case 4:
     * @require Not case1 && Not case2 && Not case3 && the EndOffset of the
     *          route in deepCopyRequested is contained in the route of
     *          requested.
     * @ensure modify EndOffset of the route of deepCopyRequested not to overlap
     *         the routes of requested.
     * 
     * 
     */
    private static List<List<Segment>> CompareWithRequested(
            List<List<Segment>> requested,
            List<List<Segment>> deepCopyRequested) {
        for (int a = 0; a < deepCopyRequested.size(); a++) {
            for (int b = 0; b < requested.size(); b++) {
                for (int c = 0; c < deepCopyRequested.get(a).size(); c++) {
                    for (int d = 0; d < requested.get(b).size(); d++) {
                        // case 1
                        if (a <= b) {
                            continue;
                        }
                        // case 2
                        else if (requested.get(b).get(d)
                                .contains(deepCopyRequested.get(a).get(c)
                                        .getFirstLocation())
                                && requested.get(b).get(d)
                                        .contains(deepCopyRequested.get(a)
                                                .get(c).getLastLocation())) {
                            deepCopyRequested.get(a).remove(c);
                            d = requested.get(b).size();
                        }
                        // case 3
                        else if (requested.get(b).get(d)
                                .contains(deepCopyRequested.get(a).get(c)
                                        .getFirstLocation())) {
                            deepCopyRequested = RequestedContainFirstLocation(a,
                                    b, c, d, requested, deepCopyRequested);

                        }
                        // case 4
                        else if (requested.get(b).get(d)
                                .contains(deepCopyRequested.get(a).get(c)
                                        .getLastLocation())) {
                            deepCopyRequested = RequestedContainLastLocation(a,
                                    b, c, d, requested, deepCopyRequested);

                        }
                    }

                }
            }
        }
        return deepCopyRequested;
    }

    /**
     * case 3: There are 3 cases to consider in case3
     * 
     * case 3-1:
     * @require StartOffset is atJunction
     * @ensure StartOffset = StartOffset + 1
     * 
     * case 3-2:
     * @require Not case 3-1 && directions of approchingPoint are same
     * @ensure StartOffset = EndOffset of occupied + 1
     * 
     * case 3-3:
     * @require Not case 3-1 && Not case 3-2 && directions of approchingPoint
     *          are different
     * @ensure StartOffset = Length - StartOffset of occupied + 1
     * 
     * 
     */
    private static List<List<Segment>> RequestedContainFirstLocation(int a,
            int b, int c, int d, List<List<Segment>> requested,
            List<List<Segment>> deepCopyRequested) {
        // case 3-1
        if (deepCopyRequested.get(a).get(c).getStartOffset() == 0) {
            deepCopyRequested.get(a).set(c, new Segment(
                    deepCopyRequested.get(a).get(c).getSection(),
                    deepCopyRequested.get(a).get(c).getDepartingEndPoint(),
                    deepCopyRequested.get(a).get(c).getStartOffset() + 1,
                    deepCopyRequested.get(a).get(c).getEndOffset()));
        } 
        // case 3-2
        else if (requested.get(b).get(d)
                .getApproachingEndPoint() == deepCopyRequested.get(a).get(c)
                        .getApproachingEndPoint()) {
            deepCopyRequested.get(a).set(c,
                    new Segment(deepCopyRequested.get(a).get(c).getSection(),
                            deepCopyRequested.get(a).get(c)
                                    .getDepartingEndPoint(),
                            requested.get(b).get(d).getEndOffset() + 1,
                            deepCopyRequested.get(a).get(c).getEndOffset()));
        } 
        // case 3-3
        else {
            deepCopyRequested.get(a).set(c, new Segment(
                    deepCopyRequested.get(a).get(c).getSection(),
                    deepCopyRequested.get(a).get(c).getDepartingEndPoint(),
                    requested.get(b).get(d).getSection().getLength()
                            - requested.get(b).get(d).getStartOffset() + 1,
                    deepCopyRequested.get(a).get(c).getEndOffset()));
        }
        return deepCopyRequested;
    }

    /**
     * case 4: There are 3 cases to consider in case4
     * 
     * case 4-1:
     * @require EndOffset is atJunction
     * @ensure EndOffset = EndOffset - 1
     * 
     * case 4-2:
     * @require Not case 4-1 && directions of approchingPoint are same
     * @ensure EndOffset = StartOffset of requested - 1
     * 
     * case 4-3:
     * @require Not case 4-1 && Not case 4-2 && directions of approchingPoint
     *          are different
     * @ensure StartOffset = Length - EndOffset of requested - 1
     *  
     */
    private static List<List<Segment>> RequestedContainLastLocation(int a,
            int b, int c, int d, List<List<Segment>> requested,
            List<List<Segment>> deepCopyRequested) {
        // case 4-1
        if (deepCopyRequested.get(a).get(c).getEndOffset() == deepCopyRequested
                .get(a).get(c).getSection().getLength()) {
            deepCopyRequested.get(a).set(c, new Segment(
                    deepCopyRequested.get(a).get(c).getSection(),
                    deepCopyRequested.get(a).get(c).getDepartingEndPoint(),
                    deepCopyRequested.get(a).get(c).getStartOffset(),
                    deepCopyRequested.get(a).get(c).getEndOffset() - 1));
        }
        // case 4-2
        else if (requested.get(b).get(d)
                .getApproachingEndPoint() == deepCopyRequested.get(a).get(c)
                        .getApproachingEndPoint()) {
            deepCopyRequested.get(a).set(c,
                    new Segment(deepCopyRequested.get(a).get(c).getSection(),
                            deepCopyRequested.get(a).get(c)
                                    .getDepartingEndPoint(),
                            deepCopyRequested.get(a).get(c).getStartOffset(),
                            requested.get(b).get(d).getStartOffset() - 1));
        } 
        // case 4-3
        else {
            deepCopyRequested.get(a).set(c,
                    new Segment(deepCopyRequested.get(a).get(c).getSection(),
                            deepCopyRequested.get(a).get(c)
                                    .getDepartingEndPoint(),
                            deepCopyRequested.get(a).get(c).getStartOffset(),
                            requested.get(b).get(d).getSection().getLength()
                                    - requested.get(b).get(d).getEndOffset()
                                    - 1));
        }
        return deepCopyRequested;
    }
}
