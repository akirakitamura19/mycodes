package railway.test;

import railway.*;
import java.util.*;
import org.junit.Assert;
import org.junit.Test;

/**
 * Basic tests for the {@link Allocator} implementation class.
 * 
 * A more extensive test suite will be performed for assessment of your code,
 * but this should get you started writing your own unit tests.
 */
public class AllocatorTest {

    /*
     * The end-points and sections of the track to be used in the tests in this
     * suite. (This isn't by any means the only track layout possible, but we
     * will use the one track for these particular tests for simplicity.
     * Your code should work for any track).
     */
    private JunctionBranch[] endPoints = {
            new JunctionBranch(new Junction("j0"), Branch.NORMAL),
            new JunctionBranch(new Junction("j1"), Branch.FACING),
            new JunctionBranch(new Junction("j1"), Branch.REVERSE),
            new JunctionBranch(new Junction("j2"), Branch.FACING),
            new JunctionBranch(new Junction("j1"), Branch.NORMAL),
            new JunctionBranch(new Junction("j3"), Branch.FACING),
            new JunctionBranch(new Junction("j2"), Branch.NORMAL),
            new JunctionBranch(new Junction("j3"), Branch.NORMAL) };

    private Section[] sections = { new Section(10, endPoints[0], endPoints[1]),
            new Section(12, endPoints[2], endPoints[3]),
            new Section(7, endPoints[4], endPoints[5]),
            new Section(10, endPoints[6], endPoints[7]) };

    /**
     * Basic test: two trains: routes with only one segment each: routes on one
     * section only: routes in same direction on section: requested allocations
     * are possible.
     */
    @Test
    public void basicTest01() throws Exception {
        // arrays describing the occupied, requested and expected routes
        Segment[][] occupiedArray =
                { { new Segment(sections[0], endPoints[0], 2, 4) },
                        { new Segment(sections[0], endPoints[0], 6, 8) } };

        Segment[][] requestedArray =
                { { new Segment(sections[0], endPoints[0], 3, 5) },
                        { new Segment(sections[0], endPoints[0], 7, 9) } };

        Segment[][] expectedAllocationArray =
                { { new Segment(sections[0], endPoints[0], 3, 5) },
                        { new Segment(sections[0], endPoints[0], 7, 9) } };

        // input parameters to method and expected outputs
        List<List<Segment>> occupied = asList(occupiedArray);
        List<List<Segment>> requested = asList(requestedArray);
        List<List<Segment>> expectedAllocation =
                asList(expectedAllocationArray);

        // the actual result of the allocation
        List<List<Segment>> actualAllocation =
                Allocator.allocate(occupied, requested);
        Assert.assertEquals(expectedAllocation, actualAllocation);
    }

    /**
     * Basic test: two trains: routes with only one segment each: routes on one
     * section only: routes in same direction on section: only part of the
     * requested allocations are possible.
     */
    @Test
    public void basicTest02() throws Exception {
        // arrays describing the occupied, requested and expected routes
        Segment[][] occupiedArray =
                { { new Segment(sections[0], endPoints[0], 0, 3) },
                        { new Segment(sections[0], endPoints[0], 6, 9) } };

        Segment[][] requestedArray =
                { { new Segment(sections[0], endPoints[0], 2, 7) },
                        { new Segment(sections[0], endPoints[0], 8, 10) } };

        Segment[][] expectedAllocationArray =
                { { new Segment(sections[0], endPoints[0], 2, 5) },
                        { new Segment(sections[0], endPoints[0], 8, 10) } };

        // input parameters to method and expected outputs
        List<List<Segment>> occupied = asList(occupiedArray);
        List<List<Segment>> requested = asList(requestedArray);
        List<List<Segment>> expectedAllocation =
                asList(expectedAllocationArray);

        // the actual result of the allocation
        List<List<Segment>> actualAllocation =
                Allocator.allocate(occupied, requested);
        Assert.assertEquals(expectedAllocation, actualAllocation);
    }

    /**
     * Basic test: two trains: routes with only one segment each: routes on one
     * section only: routes in different directions on section: requested
     * allocations are possible.
     */
    @Test
    public void basicTest03() throws Exception {
        // arrays describing the occupied, requested and expected routes
        Segment[][] occupiedArray =
                { { new Segment(sections[0], endPoints[0], 1, 3) },
                        { new Segment(sections[0], endPoints[1], 0, 3) } };

        Segment[][] requestedArray =
                { { new Segment(sections[0], endPoints[0], 2, 4) },
                        { new Segment(sections[0], endPoints[1], 1, 4) } };

        Segment[][] expectedAllocationArray =
                { { new Segment(sections[0], endPoints[0], 2, 4) },
                        { new Segment(sections[0], endPoints[1], 1, 4) } };

        // input parameters to method and expected outputs
        List<List<Segment>> occupied = asList(occupiedArray);
        List<List<Segment>> requested = asList(requestedArray);
        List<List<Segment>> expectedAllocation =
                asList(expectedAllocationArray);

        // the actual result of the allocation
        List<List<Segment>> actualAllocation =
                Allocator.allocate(occupied, requested);
        Assert.assertEquals(expectedAllocation, actualAllocation);
    }

    /**
     * Basic test: two trains: routes with only one segment each: routes on one
     * section only: routes in different directions on section: only part of the
     * requested allocations are possible.
     */
    @Test
    public void basicTest04() throws Exception {
        // arrays describing the occupied, requested and expected routes
        Segment[][] occupiedArray =
                { { new Segment(sections[0], endPoints[0], 1, 3) },
                        { new Segment(sections[0], endPoints[1], 0, 3) } };

        Segment[][] requestedArray =
                { { new Segment(sections[0], endPoints[0], 2, 4) },
                        { new Segment(sections[0], endPoints[1], 1, 7) } };

        Segment[][] expectedAllocationArray =
                { { new Segment(sections[0], endPoints[0], 2, 4) },
                        { new Segment(sections[0], endPoints[1], 1, 5) } };

        // input parameters to method and expected outputs
        List<List<Segment>> occupied = asList(occupiedArray);
        List<List<Segment>> requested = asList(requestedArray);
        List<List<Segment>> expectedAllocation =
                asList(expectedAllocationArray);

        // the actual result of the allocation
        List<List<Segment>> actualAllocation =
                Allocator.allocate(occupied, requested);
        Assert.assertEquals(expectedAllocation, actualAllocation);
    }

    /**
     * Basic test: many trains: routes with only one segment each: routes on
     * many different sections: only part of the requested allocations are
     * possible.
     */
    @Test
    public void basicTest05() throws Exception {
        // arrays describing the occupied, requested and expected routes
        Segment[][] occupiedArray =
                { { new Segment(sections[0], endPoints[0], 2, 4) },
                        { new Segment(sections[1], endPoints[3], 8, 10) },
                        { new Segment(sections[2], endPoints[5], 3, 5) },
                        { new Segment(sections[2], endPoints[5], 0, 2) } };

        Segment[][] requestedArray =
                { { new Segment(sections[0], endPoints[0], 3, 6) },
                        { new Segment(sections[1], endPoints[3], 9, 12) },
                        { new Segment(sections[2], endPoints[5], 3, 7) },
                        { new Segment(sections[2], endPoints[5], 0, 4) } };

        Segment[][] expectedAllocationArray =
                { { new Segment(sections[0], endPoints[0], 3, 6) },
                        { new Segment(sections[1], endPoints[3], 9, 12) },
                        { new Segment(sections[2], endPoints[5], 3, 6) },
                        { new Segment(sections[2], endPoints[5], 0, 2) } };

        // input parameters to method and expected outputs
        List<List<Segment>> occupied = asList(occupiedArray);
        List<List<Segment>> requested = asList(requestedArray);
        List<List<Segment>> expectedAllocation =
                asList(expectedAllocationArray);

        // the actual result of the allocation
        List<List<Segment>> actualAllocation =
                Allocator.allocate(occupied, requested);
        Assert.assertEquals(expectedAllocation, actualAllocation);
    }

    /**
     * Basic test: two trains: routes might contain more than one segment each:
     * requested allocations are possible.
     */
    @Test
    public void basicTest06() throws Exception {
        // arrays describing the occupied, requested and expected routes
        Segment[][] occupiedArray =
                {
                        { new Segment(sections[0], endPoints[0], 4, 7) },
                        { new Segment(sections[1], endPoints[2], 10, 12),
                                new Segment(sections[3], endPoints[6], 0, 4) } };

        Segment[][] requestedArray =
                {
                        { new Segment(sections[0], endPoints[0], 6, 10),
                                new Segment(sections[1], endPoints[2], 0, 3) },
                        { new Segment(sections[3], endPoints[6], 2, 7) } };

        Segment[][] expectedAllocationArray =
                {
                        { new Segment(sections[0], endPoints[0], 6, 10),
                                new Segment(sections[1], endPoints[2], 0, 3) },
                        { new Segment(sections[3], endPoints[6], 2, 7) } };

        // input parameters to method and expected outputs
        List<List<Segment>> occupied = asList(occupiedArray);
        List<List<Segment>> requested = asList(requestedArray);
        List<List<Segment>> expectedAllocation =
                asList(expectedAllocationArray);

        // the actual result of the allocation
        List<List<Segment>> actualAllocation =
                Allocator.allocate(occupied, requested);
        Assert.assertEquals(expectedAllocation, actualAllocation);
    }

    /**
     * Basic test: two trains: routes might contain more than one segment each:
     * requested allocations are partially possible.
     */
    @Test
    public void basicTest07() throws Exception {
        // arrays describing the occupied, requested and expected routes
        Segment[][] occupiedArray =
                {
                        { new Segment(sections[0], endPoints[0], 6, 10),
                                new Segment(sections[1], endPoints[2], 0, 3) },
                        { new Segment(sections[2], endPoints[4], 5, 7),
                                new Segment(sections[3], endPoints[7], 0, 1) } };

        Segment[][] requestedArray =
                {
                        { new Segment(sections[1], endPoints[2], 2, 12),
                                new Segment(sections[3], endPoints[6], 0, 5) },
                        { new Segment(sections[3], endPoints[7], 0, 10),
                                new Segment(sections[1], endPoints[3], 0, 5) } };

        Segment[][] expectedAllocationArray =
                {
                        { new Segment(sections[1], endPoints[2], 2, 12),
                                new Segment(sections[3], endPoints[6], 0, 5) },
                        { new Segment(sections[3], endPoints[7], 0, 4) } };

        // input parameters to method and expected outputs
        List<List<Segment>> occupied = asList(occupiedArray);
        List<List<Segment>> requested = asList(requestedArray);
        List<List<Segment>> expectedAllocation =
                asList(expectedAllocationArray);

        // the actual result of the allocation
        List<List<Segment>> actualAllocation =
                Allocator.allocate(occupied, requested);
        Assert.assertEquals(expectedAllocation, actualAllocation);
    }

    // -----Helper Methods-------------------------------

    /**
     * Returns a list of lists representation of the array of arrays of
     * segments.
     * 
     * @param array
     *            the array to convert
     * @return the array converted to a list of lists
     */
    private List<List<Segment>> asList(Segment[][] array) {
        List<List<Segment>> list = new ArrayList<>();
        for (Segment[] innerArray : array) {
            List<Segment> innerList = new ArrayList<>();
            for (Segment segment : innerArray) {
                innerList.add(segment);
            }
            list.add(innerList);
        }
        return list;
    }

}
