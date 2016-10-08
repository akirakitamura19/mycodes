package railway.test;

import railway.*;
import java.io.*;
import java.util.*;
import org.junit.Assert;
import org.junit.Test;

/**
 * Basic tests for the {@link TrackReader} implementation class.
 * 
 * CSSE7023: Write your own tests for the class here: I have only added a few
 * basic ones to get you started. You don't have to use these tests in your own
 * test suite.
 * 
 * A more extensive test suite will be performed for assessment of your code,
 * but this should get you started writing your own unit tests.
 */
public class TrackReaderTest {

    /**
     * Check reading a correctly formatted file with many sections.
     */
    @Test
    public void testCorrectlyFormattedManySections() throws Exception {
        Section[] sections = { new Section(9,
                new JunctionBranch(new Junction("j0"), Branch.FACING),
                new JunctionBranch(new Junction("j1"), Branch.FACING)),

                new Section(777,
                        new JunctionBranch(new Junction("j2"), Branch.NORMAL),
                        new JunctionBranch(new Junction("j4"), Branch.FACING)),

                new Section(10,
                        new JunctionBranch(new Junction("j5"), Branch.NORMAL),
                        new JunctionBranch(new Junction("j6"),
                                Branch.REVERSE)) };

        Track actualTrack = TrackReader.read("test_01_correctlyFormatted.txt");
        checkTrackSections(actualTrack,
                new HashSet<Section>(Arrays.asList(sections)));
    }

    /**
     * Check reading an incorrectly formatted file: a section is missing
     * information on a line
     */
    @Test
    public void testIncorrectlyFormattedMissingInformation() throws Exception {
        // Error on line 3: the line is blank. Each line should denote a
        // section and all section information is missing from that line.
        try {
            TrackReader.read("test_02_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }

    /**
     * Check reading not exist file.
     */
    @Test
    public void testIOExceptionMissingFile() throws Exception {
        try {
            // This file is not exist.
            TrackReader.read("test_03_MissingFile.txt");
            Assert.fail("IOException not thrown");
        } catch (IOException e) {
            // OK
        }
    }

    /**
     * Check reading a null file.
     */
    @Test
    public void testIncorrectlyFormattedNullInformation() throws Exception {
        try {
            TrackReader.read("test_04_NullFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }

    /**
     * Check reading an incorrectly formatted file: 1st element is not integer
     */
    @Test
    public void testIncorrectlyFormatted1stElement1() throws Exception {
        // Error on line 3: the 1st element is not an integer.
        try {
            TrackReader.read("test_05_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }

    /**
     * Check reading an incorrectly formatted file: 1st element is not positive
     */
    @Test
    public void testIncorrectlyFormatted1stElement2() throws Exception {
        // Error on line 3: the 1st element is negative number.
        try {
            TrackReader.read("test_06_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }

    /**
     * Check reading an incorrectly formatted file: 2nd element is missing
     */
    @Test
    public void testIncorrectlyFormatted2ndElement() throws Exception {
        // Error on line 3: the 2nd element is not exist.
        try {
            TrackReader.read("test_07_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }

    /**
     * Check reading an incorrectly formatted file: 3rd element is not a branch
     */
    @Test
    public void testIncorrectlyFormatted3rdElement() throws Exception {
        // Error on line 3: the 3nd element is not a branch.
        try {
            TrackReader.read("test_08_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }
    
    /**
     * Check reading an incorrectly formatted file: 4nd element is missing
     */
    @Test
    public void testIncorrectlyFormatted4thElement() throws Exception {
        // Error on line 3: the 4th element is not exist.
        try {
            TrackReader.read("test_09_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }
    
    /**
     * Check reading an incorrectly formatted file: 5th element is not a branch
     */
    @Test
    public void testIncorrectlyFormatted5thElement() throws Exception {
        // Error on line 3: the 5th element is not a branch.
        try {
            TrackReader.read("test_10_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }
    
    /**
     * Check reading an incorrectly formatted file: 6th element is exist
     */
    @Test
    public void testIncorrectlyFormatted6thElementExist() throws Exception {
        // Error on line 3: the 6th element is exist.
        try {
            TrackReader.read("test_11_incorrectlyFormatted.txt");
            Assert.fail("FormatException not thrown");
        } catch (FormatException e) {
            // OK
        }
    }
    

    // -----Helper Methods-------------------------------

    /**
     * Checks that the given track has all, and only the expected sections.
     * 
     * @param track
     *            The track whose sections will be checked.
     * @param expectedSections
     *            The expected sections that the track should have
     */
    private void checkTrackSections(Track track,
            Set<Section> expectedSections) {
        Set<Section> actualSections = new HashSet<>();
        for (Section section : track) {
            actualSections.add(section);
        }
        Assert.assertEquals(expectedSections, actualSections);
    }

}
