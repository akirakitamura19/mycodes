package railway;

import java.io.*;
import java.util.*;

/**
 * Provides a method to read a track from a text file.
 */
public class TrackReader {

    /**
     * <p>
     * Reads a text file named fileName that describes the sections on a track,
     * and returns a track containing each of the sections in the file.
     * </p>
     * 
     * <p>
     * The file contains zero or more lines, each of which corresponds to a
     * section on the track.
     * 
     * Each line should contain five items separated by one or more whitespace
     * characters: a positive integer representing the length of the section,
     * followed by the name of a first junction, then the type of a first
     * branch, followed by the name of a second junction, and then the type of a
     * second branch. The section denoted by the line has the given length, and
     * two end-points: one constructed from the first junction and first branch
     * on the line, and the other constructed from the second junction and
     * section branch.
     * 
     * A junction name is simply an unformatted non-empty string that doesn't
     * contain any whitespace characters. The type of a branch is one of the
     * three strings "FACING", "NORMAL" or "REVERSE", which correspond to the
     * branches Branch.FACING, Branch.NORMAL, and Branch.REVERSE, respectively.
     * 
     * There may be leading or trailing whitespace on each line of the file.
     * (Refer to the Character.isWhitespace() method for the definition of a
     * white space in java.)
     * 
     * For example, the line <br>
     * <br>
     * 
     * 10 j1 FACING j2 NORMAL
     * 
     * <br>
     * <br>
     * denotes a section with length 10 and end-points (j1, FACING) and (j2,
     * NORMAL).
     * </p>
     * 
     * <p>
     * No two lines of the file should denote equivalent sections (as defined by
     * the equals method of the Section class), and no two sections described by
     * the input file should have a common end-point (since each junction can
     * only be connected to at most one section on each branch on a valid
     * track).
     * </p>
     * 
     * <p>
     * The method throws an IOException if there is an input error with the
     * input file (e.g. the file with name given by input parameter fileName
     * does not exist); otherwise it throws a FormatException if there is an
     * error with the input format (this includes the case where there is a
     * duplicate section, and the case where two or more sections have a common
     * end-point), otherwise it returns a track that contains each of the
     * sections described in the file (and no others).
     * 
     * If a FormatException is thrown, it will have a meaningful message that
     * accurately describes the problem with the input file format, including
     * the line of the file where the problem was detected.
     * </p>
     * 
     * @param fileName
     *            the file to read from
     * @return a track containing the sections from the file
     * @throws IOException
     *             if there is an error reading from the input file
     * @throws FormatException
     *             if there is an error with the input format. The
     *             FormatExceptions thrown should have a meaningful message that
     *             accurately describes the problem with the input file format,
     *             including the line of the file where the problem was
     *             detected.
     */
    public static Track read(String fileName)
            throws IOException, FormatException {

        // Read the fileName and Scan it
        FileReader fr = new FileReader(fileName);
        Scanner scanner = new Scanner(fr);

        int i = 1;
        // Create an empty track to contain
        Track track = new Track();

        // In case of null in file, throw FormatException
        if (!(scanner.hasNextLine())) {
            scanner.close();
            throw new FormatException("There are no lines in this file.");
        }
        // While loop during existing a next line
        else {
            while (scanner.hasNextLine()) {
                // Convert each line to string and scan it
                String lineString = scanner.nextLine();
                Scanner ls = new Scanner(lineString);

                // Create each empty length, junction1, junction2, branch1 and
                // branch2 to contain
                int length = 0;
                Junction junction1 = null;
                Junction junction2 = null;
                Branch branch1 = null;
                Branch branch2 = null;

                // Check whether 1st element is an positive integer.
                // otherwise throw FormatException
                if (ls.hasNextInt()) {
                    length = ls.nextInt();
                    if (length <= 0) {
                        ls.close();
                        throw new FormatException("1st element in line number "
                                + i + " has to be positive.");
                    }
                } else {
                    ls.close();
                    throw new FormatException("1st element in line number " + i
                            + " has to be an integer.");
                }

                // Check whether 2nd element is exist for junction.
                // otherwise throw FormatException
                if (ls.hasNext()) {
                    junction1 = new Junction(ls.next());
                } else {
                    ls.close();
                    throw new FormatException("2nd element in line number " + i
                            + " has to be" + " an exist to be junction.");
                }

                // Check whether 3rd element is branch. otherwise throw
                // FormatException
                String thirdElement = ls.next();
                if (thirdElement.equals("FACING")) {
                    branch1 = Branch.FACING;
                } else if (thirdElement.equals("NORMAL")) {
                    branch1 = Branch.NORMAL;
                } else if (thirdElement.equals("REVERSE")) {
                    branch1 = Branch.REVERSE;
                } else {
                    ls.close();
                    throw new FormatException("3rd element in line number " + i
                            + " has to be branch.");
                }

                // Check whether 4th element is exist for junction.
                // otherwise throw FormatException
                if (ls.hasNext()) {
                    junction2 = new Junction(ls.next());
                } else {
                    ls.close();
                    throw new FormatException("4th element in line number " + i
                            + " has to be" + " an exist to be junction.");
                }

                // Check whether 5th element is branch. otherwise throw
                // FormatException
                String fifthElement = ls.next();
                if (fifthElement.equals("FACING")) {
                    branch2 = Branch.FACING;
                } else if (fifthElement.equals("NORMAL")) {
                    branch2 = Branch.NORMAL;
                } else if (fifthElement.equals("REVERSE")) {
                    branch2 = Branch.REVERSE;
                } else {
                    ls.close();
                    throw new FormatException("5th element in line number " + i
                            + " has to be branch.");
                }

                // Check whether 6th element is not exist. otherwise throw
                // FormatException
                if (!(ls.hasNext())) {
                    ls.close();
                } else {
                    ls.close();
                    throw new FormatException("Line number " + i
                            + " has to be consisted in only 5 elements");
                }

                // Contain each elements into endPoint1, endPoint2 and section
                // Add section into track
                JunctionBranch endPoint1 = new JunctionBranch(junction1,
                        branch1);
                JunctionBranch endPoint2 = new JunctionBranch(junction2,
                        branch2);
                Section section = new Section(length, endPoint1, endPoint2);
                track.addSection(section);
                i ++;

            }
        }
        scanner.close();
        return track;
    }

}
