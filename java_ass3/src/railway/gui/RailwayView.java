package railway.gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * The view for the Railway Manager.
 */
@SuppressWarnings("serial")
public class RailwayView extends JFrame {


    // The font for showing up texts
    private Font font = new Font("SanSerif", Font.BOLD, 12);
    // The button for adding an new train
    private JButton addTrain;
    // The button for updating an train start & endOffsets
    private JButton updateTrain;
    // The button for viewing the result depending on the selected train
    private JButton viewTrain;

    /**
     * Creates a new Railway Manager window.
     */
    public RailwayView(RailwayModel model) {
        setTitle("RailwayManager");
        setBounds(400, 200, 400, 200);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        Container c = getContentPane();
        addButtons(c);
    }

    /**
     * Creates 3 buttons on Railway Manager window.
     */
    private void addButtons(Container c) {
        JPanel p = new JPanel();
        addTrain(p);
        updateTrain(p);
        viewTrain(p);
        c.add(p, "Center");
    }

    /**
     * Create "Add Train" buttons.
     */
    private void addTrain(JPanel p) {
        addTrain = new JButton("Add Train");
        addTrain.setFont(font);
        addTrain.setBackground(Color.WHITE);
        addTrain.setForeground(Color.black);
        p.add(addTrain);
    }

    /**
     * Create "Update Train" buttons.
     */
    private void updateTrain(JPanel p) {
        updateTrain = new JButton("Update Train");
        updateTrain.setFont(font);
        updateTrain.setBackground(Color.WHITE);
        updateTrain.setForeground(Color.black);
        p.add(updateTrain);
    }

    /**
     * Create "View Result" buttons.
     */
    private void viewTrain(JPanel p) {
        viewTrain = new JButton("View Result");
        viewTrain.setFont(font);
        viewTrain.setBackground(Color.WHITE);
        viewTrain.setForeground(Color.black);
        p.add(viewTrain);
    }

    /**
     * Add listeners for "Add Train" button.
     */
    public void addTrainListener(ActionListener pl) {
        addTrain.addActionListener(pl);
    }

    /**
     * Add listeners for "Update Train" button.
     */
    public void updateTrainListener(ActionListener pl) {
        updateTrain.addActionListener(pl);
    }

    /**
     * Add listeners for "View Result" button.
     */
    public void viewTrainListener(ActionListener pl) {
        viewTrain.addActionListener(pl);
    }

    /**
     * Create a pop-up to show meaningful massage after loading a track file.
     * 
     * @param Message
     *          What is working correctly.
     */
    public void showInfoMessage(String e) {
        Container c = getContentPane();
        JOptionPane.showMessageDialog(c, e, "Info",
                JOptionPane.INFORMATION_MESSAGE);
    }

    /**
     * Create a pop-up to show meaningful error massage when getting error.
     * 
     * @param Message
     *          What is wrong.
     */
    public void showErrorMessage(String e) {
        Container c = getContentPane();
        JOptionPane.showMessageDialog(c, e, "Error", JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Show up pop-up window to input the route when pushing Add button.
     * 
     * @return Filename of the route
     */
    public String inputRoute() {
        String filename = JOptionPane.showInputDialog(
                "What is the name of route file?", "ex)route0.txt");
        return filename;
    }

    /**
     * Show up pop-up window to input StartOffset.
     * 
     * @return StartOffset
     */
    public int inputStartOffset() throws NumberFormatException {
        String inputNumber = JOptionPane.showInputDialog("Input <startOffset>");
        int startOffset = Integer.parseInt(inputNumber);
        return startOffset;
    }

    /**
     * Show up pop-up window to input EndOffset.
     * 
     * @return EndOffset
     */
    public int inputEndOffset() throws NumberFormatException {
        String inputNumber = JOptionPane.showInputDialog("Input <endOffset>");
        int endOffset = Integer.parseInt(inputNumber);
        return endOffset;
    }

    /**
     * Show up pop-up window to input the identifier.
     * 
     * @return Identifier
     */
    public int inputIdentifier(String e) {
        String inputNumber = JOptionPane.showInputDialog(e);
        int identifier = Integer.parseInt(inputNumber);
        return identifier;
    }
}
